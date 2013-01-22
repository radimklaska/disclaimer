<?php

/**
 * @file
 * Admin page callbacks for the disclaimer module.
 *
 */

/*
 * Admin settings form for disclaimer.
 *
 */
function disclaimer_admin_settings() {
  $form['disclaimer_preview'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer preview'),
  );
  $form['disclaimer_preview']['content'] = array(
    '#type' => 'markup',
    '#markup' => _disclaimer_get_content(),
  );
  // build content selection form for content and footer
  $form += _disclaimer_content_selection();
  $form += _disclaimer_content_selection('footer');

  $form['action'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer action'),
  );
  $form['action']['disclaimer_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show disclaimer on specific pages'),
      '#options' => array(
        t('Show disclaimer on every page except the listed pages.'),
        t('Show disclaimer on only the listed pages.'),
      ),
      '#default_value' => variable_get('disclaimer_visibility', 0),
  );
  $form['action']['disclaimer_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Disclaimer specific pages'),
      '#default_value' => variable_get('disclaimer_pages', "admin/*\nuser"),
      '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  $form['action']['disclaimer_enter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter url'),
    '#default_value' => variable_get('disclaimer_enter_url', ''),
    '#description' => t('Set the url when press Enter on disclaimer. IF EMPTY: modal is just closed without redirection. You can use Drupal system path.'),
  );
  $form['action']['disclaimer_exit_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit url'),
    '#default_value' => variable_get('disclaimer_exit_url', 'http://www.google.com'),
    '#description' => t('Set the url when press Exit on disclaimer. Default is "http://www.google.com".'),
  );
  $form['action']['disclaimer_age_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Age verification form.'),
    '#default_value' => variable_get('disclaimer_age_form', 0),
    '#return_value' => 1,
    '#description' => t('CAUTION: if you test modal on this current settings page, age verification will not work because of preview window.'),
  );
  $form['action']['disclaimer_age_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Select age limit'),
    '#field_suffix' => t('years old'),
    '#default_value' => variable_get('disclaimer_age_limit', 18),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('If you add age verification form, you can set age limit.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_age_form"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['style'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer action style'),
  );
  $form['style']['disclaimer_action_type'] = array(
    '#type' => 'select',
    '#title' => t('Disclaimer action style'),
    '#default_value' => variable_get('disclaimer_action_type', 'text'),
    '#options' => array(
      'text' => t('Use text'),
      'image' => t('Use images'),
    ),
    '#description' => t('Select what do you want to use for "ENTER" or "EXIT" action on the disclaimer.'),
  );
  // Actions options
  $types = array(
    'enter' => array(
      'id' => 'enter',
      'name' => t('Enter'),
    ),
    'exit' => array(
      'id' => 'exit',
      'name' => t('Exit'),
    ),
  );
    // Build text setting
  $form['style']['text'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_action_type"]' => array('value' => 'text'),
      ),
    ),
  );
  foreach ($types AS $type) {
    $form['style']['text']['disclaimer_' . $type['id'] . '_txt'] = array(
      '#type' => 'textfield',
      '#title' => t('Disclaimer !name text', array('!name' => $type['name'])),
      '#size' => 20,
      '#default_value' => variable_get('disclaimer_' . $type['id'] . '_txt', $type['name']),
      '#description' => t('Set the text for this action on disclaimer.'),
    );
  }
  // Build images setting
  $form['style']['img'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_action_type"]' => array('value' => 'image'),
      ),
    ),
  );
  // get image styles for options
  $options = image_style_options();
  foreach ($types AS $type) {
    $form['style']['img'][$type['id']] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#title' => t('!name action style', array('!name' => $type['name'])),
    );
    // Use the #managed_file FAPI element to upload an image file.
    $form['style']['img'][$type['id']]['disclaimer_' . $type['id'] . '_img'] = array(
      '#type' => 'managed_file',
      '#title' => t('Image'),
      '#description' => t('Image to be used for this action button.'),
      '#default_value' => variable_get('disclaimer_' . $type['id'] . '_img', ''),
      '#upload_location' => 'public://images_disclaimer/',
    );
    $form['style']['img'][$type['id']]['disclaimer_' . $type['id'] . '_img_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#description' => t('Select the style to render image.'),
      '#default_value' => variable_get('disclaimer_' . $type['id'] . '_img_style', ''),
      '#options' => $options,
    );
    $form['style']['img'][$type['id']]['disclaimer_' . $type['id'] . '_img_alt'] = array(
      '#type' => 'textfield',
      '#title' => t('Alt text'),
      '#size' => 20,
      '#default_value' => variable_get('disclaimer_' . $type['id'] . '_img_alt', $type['name']),
      '#description' => t('Image alt/title text to be used.'),
    );
  }
  $form['modal'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer colorbox options'),
  );
  $form['modal']['disclaimer_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Max width'),
    '#default_value' => variable_get('disclaimer_width', '80%'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Set a maximum width for loaded content. Example: "100%", 500, "500px".'),
  );
  $form['modal']['disclaimer_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#default_value' => variable_get('disclaimer_height', '80%'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Set a maximum height for loaded content. Example: "100%", 500, "500px".'),
  );
  $form['modal']['disclaimer_initialwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial width'),
    '#default_value' => variable_get('disclaimer_initialwidth', '300'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Set the initial width, prior to any content being loaded. Example: "100%", 500, "500px".'),
  );
  $form['modal']['disclaimer_initialheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial height'),
    '#default_value' => variable_get('disclaimer_initialheight', '250'),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Set the initial height, prior to any content being loaded. Example: "100%", 500, "500px".'),
  );
  $form['disclaimer_advanced'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Disclaimer advanced options'),
  );
  $form['disclaimer_advanced']['disclaimer_logged'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show disclaimer when logged user came back.'),
    '#default_value' => variable_get('disclaimer_logged', 1),
    '#description' => t('Show disclaimer when user has new browser session. Note: this option is complementary to <a href="!url">bypass disclaimer access</a>.', array('!url' => url('admin/people/permissions', array('fragment' => 'module-disclaimer')))),
  );
  $form['disclaimer_advanced']['disclaimer_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie name'),
    '#default_value' => variable_get('disclaimer_cookie_name', 'disclaimerShow'),
    '#description' => t('Set the cookie name. Default is "disclaimerShow".'),
    '#required' => TRUE
  );
  $form['disclaimer_advanced']['disclaimer_cookie_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie path'),
    '#default_value' => variable_get('disclaimer_cookie_path', '/'),
    '#description' => t('Set the cookie path. Default is "/" (recommended for access on all your site). Empty get server path.'),
  );
  /*
  $form['disclaimer_advanced']['disclaimer_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie domain'),
    '#default_value' => variable_get('disclaimer_cookie_domain', ''),
    '#description' => t('Set the cookie domain. Empty set default server domain.'),
  );
  */
  return system_settings_form($form);
}

/*
 * Build content selection part of settings form
 * @return array
 *   an array of forms
 *
 */
function _disclaimer_content_selection($zone = 'content') {
  $options = array();
  $default = '';
  // get all view modes
  $entity_info = entity_get_info('node');
  foreach ($entity_info['view modes'] AS $key => $view_mode) {
    $options[$key] = $view_mode['label'];
  }

  if ($zone == 'footer') {
    $wrap = 'content_footer';
    $key = 'disclaimer_footer_';
    $title = t('Disclaimer footer');
  }
  else {
    $wrap = 'content';
    $key = 'disclaimer_main_';
    $title = t('Disclaimer main content');
  }

  // build form
  $form[$wrap] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => $title,
  );
  $form[$wrap][$key . 'source'] = array(
    '#type' => 'select',
    '#title' => t('Content source'),
    '#options' => array(
      0 => t('Custom text'),
      1 => t('Existing node'),
    ),
    '#default_value' => variable_get($key . 'source', 0),
    '#description' => t('You can select the source of this content, custom text or existing node.'),
  );
  // Get default node title
  if (variable_get($key . 'node', NULL)) {
    $node = node_load(variable_get($key . 'node'));
    if (isset($node->nid)) {
      $default = $node->title . ' [' . $node->nid . ']';
    }
  }
  $form[$wrap][$key . 'node'] = array(
    '#type' => 'textfield',
    '#description' => t('Type a title to search a node to use for this content.'),
    '#title' => t('Node'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'node/autocomplete',
    '#default_value' => $default,
    '#states' => array(
      'visible' => array(
        ':input[name="' . $key . 'source"]' => array('value' => '1'),
      ),
    ),
  );
  $form[$wrap][$key . 'node_view'] = array(
    '#type' => 'select',
    '#title' => t('View mode'),
    '#description' => t('Choose node view mode to use for this node.'),
    '#options' => $options,
    '#default_value' => variable_get($key . 'node_view', 'full'),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $key . 'source"]' => array('value' => '1'),
      ),
    ),
  );
  $form[$wrap][$key . 'node_view_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show links'),
    '#default_value' => variable_get($key . 'node_view_links'),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $key . 'source"]' => array('value' => '1'),
      ),
    ),
  );
  $form[$wrap]['container'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="' . $key . 'source"]' => array('value' => '0'),
      ),
    ),
  );
  $content = variable_get($key . 'content', array('value' => ''));
  $form[$wrap]['container'][$key . 'content'] = array(
    '#type' => 'text_format',
    '#default_value' => $content['value'],
    '#format' => isset($content['format']) ? $content['format'] : NULL,
  );

  return $form;
}

/**
 * Validation function for the general configuration form.
 *
 */
function disclaimer_admin_settings_validate($form, &$form_state) {
  // Validate content and footer
  if ($form_state['values']['disclaimer_main_source'] == 1) {
    if (trim($form_state['values']['disclaimer_main_node']) == '') {
      form_set_error('disclaimer_main_node', t('Please select a node.'));
    }
    else {
      $form_state['values']['disclaimer_main_node'] = _disclaimer_validate_title($form, $form_state, 'disclaimer_main_node');
    }
  }
  if ($form_state['values']['disclaimer_footer_source'] == 1) {
    if (trim($form_state['values']['disclaimer_footer_node']) == '') {
      form_set_error('disclaimer_footer_node', t('Please select a node.'));
    }
    else {
      $form_state['values']['disclaimer_footer_node'] = _disclaimer_validate_title($form, $form_state, 'disclaimer_footer_node');
    }
  }
  // validate image selection
  if ($form_state['values']['disclaimer_action_type'] == 'image') {
    if (!isset($form_state['values']['disclaimer_enter_img']) || $form_state['values']['disclaimer_enter_img'] == 0) {
      form_set_error('disclaimer_enter_img', t('Please select an image to upload.'));
    }
    if (!isset($form_state['values']['disclaimer_exit_img']) || $form_state['values']['disclaimer_exit_img'] == 0) {
      form_set_error('disclaimer_exit_img', t('Please select an image to upload.'));
    }
  }
}

/**
 * Validation function for the node autocomplete widget.
 *
 */
function _disclaimer_validate_title($form, $form_state, $key = '') {
  $title = $form_state['values'][$key];
  $matches = array();
  $nid = 0;

  // This preg_match() looks for the last pattern like [33334] and if found extracts the numeric portion.
  $result = preg_match('/\[([0-9]+)\]$/', $title, $matches);
  if ($result > 0) {
    // If $result is nonzero, we found a match and can use it as the index into $matches.
    $nid = $matches[$result];
    // Verify that it's a valid nid.
    $node = node_load($nid);
    if (empty($node)) {
      form_error($form[$key], t('Sorry, no node with nid %nid can be found', array('%nid' => $nid)));
      return;
    }
  }

  // Now, if we somehow found a nid, assign it to the node. If we failed, emit an error.
  if (!empty($nid)) {
    return $nid;
  }
  else {
    form_error($form[$key], t('Sorry, no node starting with %title can be found', array('%title' => $title)));
  }
}
