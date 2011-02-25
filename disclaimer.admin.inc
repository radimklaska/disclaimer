<?php

/**
 * @file
 * Admin page callbacks for the disclaimer module.
 */

/* Setup settings form for disclaimer
 * @return array an array of forms
 */
function disclaimer_admin_settings() {
  global $user;
  // preview disclaimer with no js
  $disclaimer = disclaimer_get_content(FALSE);
  $width = variable_get('disclaimer_width', 400);
  $height = variable_get('disclaimer_height', 300);
  $node_query = db_query('SELECT n.nid, n.title FROM {node} n WHERE nid > :nid', array(':nid' => 0));
  $options = array(t('- None, use content form below -'));
  foreach ($node_query as $row) {
    $options[$row->nid] = $row->title . ' (' . drupal_get_path_alias('node/' . $row->nid) . ')';
  }
  $path = drupal_get_path('module', 'disclaimer');
  if (file_exists($path . '/nyroModal/js/jquery.nyroModal.js')) {
    $optionsmodal['nyroModal'] = 'nyroModal';
  }
  if (file_exists($path . '/jqModal/jqModal.js') && file_exists($path . '/jqModal/jqModal.css')) {
    $optionsmodal['jqModal'] = 'jqModal';
  }
  if (empty($optionsmodal)) {
    $optionsmodal[] = t('No modal script found');
    if ($user->uid == 1) {
      drupal_set_message(t('Can\'t find any modal script. Please see INSTALL.txt.'), 'error');
    }
  }
  $form['disclaimer_preview'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer preview'),
  );
  $form['disclaimer_preview']['content'] = array(
    '#type' => 'markup',
    '#markup' => $disclaimer,
  );
  $form['content'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer content'),
  );
  $form['content']['disclaimer_node'] = array(
    '#type' => 'select',
    '#title' => t('Node to use for disclaimer content'),
    '#options' => $options,
    '#default_value' => variable_get('disclaimer_node', 0),
    '#description' => t('You can select a node to use for the body of the disclaimer.'),
  );
  $form['content']['container'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_node"]' => array('value' => '0'),
      ),
    ),
  );
  $content = variable_get('disclaimer_content', array('value' => ''));
  $form['content']['container']['disclaimer_content'] = array(
    '#type' => 'text_format',
    '#default_value' => $content['value'],
    '#format' => isset($content['format']) ? $content['format'] : NULL,
    '#description' => t('Optional: If you don\'t select a node, you can use this field for the body of the disclaimer. You can use HTML and choose input filter properly.'),
  );
  $form['content']['disclaimer_node_footer'] = array(
    '#type' => 'select',
    '#title' => t('Node to use for disclaimer footer'),
    '#options' => $options,
    '#default_value' => variable_get('disclaimer_node_footer', 0),
    '#description' => t('You can select a node to use for the footer of the disclaimer.'),
  );
  $form['content']['container-footer'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_node_footer"]' => array('value' => '0'),
      ),
    ),
  );
  $content = variable_get('disclaimer_content_footer', array('value' => ''));
  $form['content']['container-footer']['disclaimer_content_footer'] = array(
    '#type' => 'text_format',
    '#default_value' => $content['value'],
    '#description' => t('Optional: If you don\'t select a node, you can use this field for the footer of the disclaimer, under ENTER/EXIT. you can use HTML and choose input filter properly.'),
    '#format' => isset($content['format']) ? $content['format'] : NULL,
  );
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
      '#default_value' => variable_get('disclaimer_pages', ''),
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
    '#description' => t('CAUTION: if you test modal on this current settings page, age verification won\'t work because of preview window.'),
  );
  $form['action']['disclaimer_age_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Select age limit'),
    '#field_suffix' => t('years old'),
    '#default_value' => variable_get('disclaimer_age_limit', 18),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('If you add age verification form, you can set age limit.'),
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
  $form['style']['action']['disclaimer_action_type'] = array(
    '#type' => 'select',
    '#title' => t('Disclaimer action style'),
    '#default_value' => variable_get('disclaimer_action_type', 'text'),
    '#options' => array(
      'text' => t('Use text'),
      'image' => t('Use images'),
    ),
    '#description' => t('Select what do you want to use for "ENTER" or "EXIT" action on the disclaimer.'),
  );
  $form['style']['action']['text'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_action_type"]' => array('value' => 'text'),
      ),
    ),
  );
  $form['style']['action']['text']['disclaimer_enter_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_enter_txt', t('ENTER')),
    '#description' => t('Set the text for Enter action on disclaimer. Default is "ENTER"'),
  );
  $form['style']['action']['text']['disclaimer_exit_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_exit_txt', t('EXIT')),
    '#description' => t('Set the text for Exit action on disclaimer. Default is "EXIT"'),
  );
  $form['style']['action']['img'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_action_type"]' => array('value' => 'image'),
      ),
    ),
  );
  // Use the #managed_file FAPI element to upload an image file.
  $form['style']['action']['img']['disclaimer_enter_img'] = array(
    '#type' => 'managed_file',
    '#title' => t('Disclaimer enter image'),
    '#description' => t('Set the image to be used for !action action on disclaimer.', array('!action' => t('Enter'))),
    '#default_value' => variable_get('disclaimer_enter_img', ''),
    '#upload_location' => 'public://images_disclaimer/',
  );
  $form['style']['action']['img']['disclaimer_enter_img_alt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter image alt/title text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_enter_img_alt', 'ENTER'),
    '#description' => t('Set the image alt/title text to be used for Enter action on disclaimer. Default is "Enter".'),
  );
  $form['style']['action']['img']['disclaimer_exit_img'] = array(
    '#type' => 'managed_file',
    '#title' => t('Disclaimer exit image'),
    '#description' => t('Set the image to be used for !action action on disclaimer.', array('!action' => t('Exit'))),
    '#default_value' => variable_get('disclaimer_exit_img', ''),
    '#upload_location' => 'public://images_disclaimer/',
  );
  $form['style']['action']['img']['disclaimer_exit_img_alt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit image alt/title text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_exit_img_alt', 'EXIT'),
    '#description' => t('Set the image alt/title text to be used for Exit action on disclaimer. Default is "Enter".'),
  ); 
  $form['modal'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer modal options'),
  );
  $form['modal']['disclaimer_modal'] = array(
    '#type' => 'select',
    '#title' => t('Choose modal jQuery plugin'),
    '#default_value' => variable_get('disclaimer_modal', 'nyroModal'),
    '#options' => $optionsmodal,
    '#description' => t('Select the modal script you want to use. See INSTALL.txt for instalation and compare.'),
  );
  $form['modal']['disclaimer_options_nyromodal'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Disclaimer nyroModal options'),
    '#description' => t('Visit !url for settings explanation.', array('!url' => l('http://nyromodal.nyrodev.com/', 'http://nyromodal.nyrodev.com/', array('html' => TRUE)))),
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_modal"]' => array('value' => 'nyroModal'),
      ),
    ),
  );
  $form['modal']['disclaimer_options_nyromodal']['disclaimer_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum width'),
    '#field_suffix' => t('px'),
    '#default_value' => variable_get('disclaimer_width', 400),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('The minimum width of the lightbox containing the disclaimer. Default 400px.'),
  );
  $form['modal']['disclaimer_options_nyromodal']['disclaimer_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum height'),
    '#field_suffix' => t('px'),
    '#default_value' => variable_get('disclaimer_height', 300),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('The minimum height of the lightbox containing the disclaimer. Default 300px.'),
  );
  $form['modal']['disclaimer_options_nyromodal']['disclaimer_padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding'),
    '#field_suffix' => t('px'),
    '#default_value' => variable_get('disclaimer_padding', 20),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('The padding of the lightbox containing the disclaimer. Default 20px.'),
  );
  $form['modal']['disclaimer_options_nyromodal']['disclaimer_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#field_prefix' => '#',
    '#default_value' => variable_get('disclaimer_bgcolor', '000000'),
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('The hex color of the lightbox background. Default #000000.'),
  );
  $form['modal']['disclaimer_options_nyromodal']['disclaimer_debug'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#default_value' => variable_get('disclaimer_debug', 'false'),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#description' => t('Debug mode for the NyroModal.'),
  );
  $form['modal']['disclaimer_options_jqmodal'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Disclaimer jqModal options'),
    '#description' => t('Visit !url for settings explanation.', array('!url' => l('http://dev.iceburg.net/jquery/jqModal/', 'http://dev.iceburg.net/jquery/jqModal/', array('html' => TRUE)))),
    '#states' => array(
      'visible' => array(
        ':input[name="disclaimer_modal"]' => array('value' => 'jqModal'),
      ),
    ),
  );
  $form['modal']['disclaimer_options_jqmodal']['disclaimer_jqmodal_overlay'] = array(
    '#type' => 'textfield',
    '#title' => t('Background overlay'),
    '#field_suffix' => t('%'),
    '#default_value' => variable_get('disclaimer_jqmodal_overlay', 90),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('The background overlay of the modal. Default 90%.'),
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
    '#description' => t('Show disclaimer for auto-login user on a new session. Default is yes. Note: this option is complementary to "bypass disclaimer" access.'),
  );
  $form['disclaimer_advanced']['cookie'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Disclaimer cookie options'),
    '#description' => t('EXPERIMENTAL: Use with CAUTION.'),
  );
  $form['disclaimer_advanced']['cookie']['disclaimer_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie name'),
    '#default_value' => variable_get('disclaimer_cookie_name', 'disclaimerShow'),
    '#description' => t('Set the cookie name. Default is "disclaimerShow".'),
    '#required' => TRUE
  );
  $form['disclaimer_advanced']['cookie']['disclaimer_cookie_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie path'),
    '#default_value' => variable_get('disclaimer_cookie_path', '/'),
    '#description' => t('Set the cookie path. Default is "/" (recommended for access on all your site). Empty get server path.'),
  );
  $form['disclaimer_advanced']['cookie']['disclaimer_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer cookie domain'),
    '#default_value' => variable_get('disclaimer_cookie_domain', ''),
    '#description' => t('Set the cookie domain. Empty set default server domain.'),
  );
  //return system_settings_form($form);
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Validation function for the general configuration form.
 *
 * Ensure that numeric value are numeric... and set input format
 */
function disclaimer_admin_settings_validate($form, &$form_state) {
  if ($form_state['values']['disclaimer_action_type'] == 'image') {
    if (!isset($form_state['values']['disclaimer_enter_img']) || $form_state['values']['disclaimer_enter_img'] == 0) {
      form_set_error('disclaimer_enter_img', t('Please select an image to upload.'));
    }
    if (!isset($form_state['values']['disclaimer_exit_img']) || $form_state['values']['disclaimer_exit_img'] == 0) {
      form_set_error('disclaimer_exit_img', t('Please select an image to upload.'));
    }
  }

/*
  $height = $form_state['values']['disclaimer_height'];
  $width = $form_state['values']['disclaimer_width'];
  $padding = $form_state['values']['disclaimer_padding'];
  $bgcolor = $form_state['values']['disclaimer_bgcolor'];
  $age_limit = $form_state['values']['disclaimer_age_limit'];
  $overlay = $form_state['values']['disclaimer_jqmodal_overlay'];

  if (!empty($padding) && !is_numeric($padding)) {
    form_set_error('disclaimer_padding', t('You must enter a numeric padding.'));
  }
  if (empty($bgcolor)) {
    form_set_error('disclaimer_bgcolor', t('You must enter a bgcolor.'));
  }
  if (!empty($width) && (!is_numeric($width) || $width < 0)) {
    form_set_error('disclaimer_width', t('You must enter a width greater than 0 pixels.'));
  }
  if (!empty($height) && (!is_numeric($height) || $height < 0)) {
    form_set_error('disclaimer_height', t('You must enter a height greater than 0 pixels.'));
  }
  if (!empty($age_limit) && (!is_numeric($age_limit) || $age_limit < 0)) {
    form_set_error('disclaimer_age_limit', t('You must enter an age greater than 0.'));
  }
  if (!empty($overlay) && !is_numeric($overlay)) {
    form_set_error('disclaimer_jqmodal_overlay', t('You must enter a numeric overlay.'));
  }
  cache_clear_all();
  */
}

function disclaimer_admin_settings_submit($form, &$form_state) {
  unset($form_state['values']['form_id'], $form_state['values']['form_token'], $form_state['values']['form_build_id'], $form_state['values']['op'], $form_state['values']['submit']);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
  // files
  if ($form_state['values']['disclaimer_action_type'] == 'image') {
    if ($form_state['values']['disclaimer_enter_img'] != 0) {
      $file = file_load($form_state['values']['disclaimer_enter_img']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
    }
    if ($form_state['values']['disclaimer_exit_img'] != 0) {
      $file = file_load($form_state['values']['disclaimer_exit_img']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

