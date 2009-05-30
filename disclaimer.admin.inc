<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the disclaimer module.
 */

/* Setup settings form for disclaimer
 * @return array an array of forms
 */
function disclaimer_admin_settings() {
  global $user;
  $disclaimer = _disclaimer_get_content();
  $width = variable_get('disclaimer_width', 400);
  $height = variable_get('disclaimer_height', 300);
  $node_query = db_query('SELECT n.nid FROM {node} n WHERE nid > 0');
  $options = array(t('- None, use content form below -'));
  while ($row = db_fetch_object($node_query)) {
    $options[$row->nid] = drupal_get_path_alias('node/'. $row->nid);
  }
  $path = drupal_get_path('module', 'disclaimer');
  if (file_exists($path .'/nyroModal/js/jquery.nyroModal.js')) {
    $optionsmodal['nyroModal'] = 'nyroModal';
  }
  if (file_exists($path .'/jqModal/jqModal.js')) {
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
    '#value' => $disclaimer,
  );
  $form['disclaimer_html'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer content'),
  );
  $form['disclaimer_html']['disclaimer_node'] = array(
    '#type' => 'select',
    '#title' => t('Node to use for disclaimer content'),
    '#options' => $options,
    '#default_value' => variable_get('disclaimer_node', 0),
    '#description' => t('You can select a node to use for the body of the disclaimer.'),
  );
  $form['disclaimer_html']['disclaimer_content'] = array(
    '#type' => 'textarea',
    '#title' => t('OR disclaimer content'),
    '#default_value' => variable_get('disclaimer_content', ''),
    '#description' => t('Optional: If you don\'t select a node, you can use this field for the body of the disclaimer. You can use HTML and choose input filter properly.'),
  );
  $form['disclaimer_html']['disclaimer_input_format'] = filter_form(variable_get('disclaimer_content_format', 1), NULL, array('disclaimer_input_format'));
  $form['disclaimer_html']['disclaimer_node_footer'] = array(
    '#type' => 'select',
    '#title' => t('Node to use for disclaimer footer'),
    '#options' => $options,
    '#default_value' => variable_get('disclaimer_node_footer', 0),
    '#description' => t('You can select a node to use for the footer of the disclaimer.'),
  );
  $form['disclaimer_html']['disclaimer_content_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('OR disclaimer footer'),
    '#default_value' => variable_get('disclaimer_content_footer', ''),
    '#description' => t('Optional: If you don\'t select a node, you can use this field for the footer of the disclaimer, under ENTER/EXIT. you can use HTML and choose input filter properly.'),
  );
  $form['disclaimer_html']['disclaimer_footer_input_format'] = filter_form(variable_get('disclaimer_content_footer_format', 1), NULL, array('disclaimer_footer_input_format'));
  $form['disclaimer_action'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer action'),
  );
  $form['disclaimer_action']['disclaimer_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show disclaimer on specific pages'),
      '#options' => array(
        t('Show disclaimer on every page except the listed pages.'),
        t('Show disclaimer on only the listed pages.'),
      ),
      '#default_value' => variable_get('disclaimer_visibility', 0),
  );
  $form['disclaimer_action']['disclaimer_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Disclaimer specific pages'),
      '#default_value' => variable_get('disclaimer_pages', ''),
      '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  $form['disclaimer_action']['disclaimer_enter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter url'),
    '#default_value' => variable_get('disclaimer_enter_url', ''),
    '#description' => t('Set the url when press Enter on disclaimer. IF EMPTY: modal is just closed without redirection. You can use Drupal system path.'),
  );
  $form['disclaimer_action']['disclaimer_exit_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit url'),
    '#default_value' => variable_get('disclaimer_exit_url', 'http://www.google.com'),
    '#description' => t('Set the url when press Exit on disclaimer. Default is "http://www.google.com".'),
  );
  $form['disclaimer_action_style'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Disclaimer action style'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['disclaimer_action_type'] = array(
    '#type' => 'select',
    '#title' => t('Disclaimer action style'),
    '#default_value' => variable_get('disclaimer_action_type', 'text'),
    '#options' => array(
      'text' => t('Use text'),
      'image' => t('Use images'),
    ),
    '#description' => t('Select what do you want to use for "ENTER" or "EXIT" action on the disclaimer.'),
  );
  $form['disclaimer_action']['disclaimer_age_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Age verification form.'),
    '#default_value' => variable_get('disclaimer_age_form', 0),
    '#return_value' => 1,
    '#description' => t('CAUTION: if you test modal on this current settings page, age verification won\'t work because of preview window.'),
  );
  $form['disclaimer_action']['disclaimer_age_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Select age limit'),
    '#field_suffix' => t('years old'),
    '#default_value' => variable_get('disclaimer_age_limit', 18),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('If you add age verification form, you can set age limit.'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['text'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Text settings'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['text']['disclaimer_enter_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_enter_txt', t('ENTER')),
    '#description' => t('Set the text for Enter action on disclaimer. Default is "ENTER"'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['text']['disclaimer_exit_txt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_exit_txt', t('EXIT')),
    '#description' => t('Set the text for Exit action on disclaimer. Default is "EXIT"'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['img'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Image settings'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['img']['disclaimer_enter_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter image name'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_enter_img', 'enter.png'),
    '#description' => t('Set the image name to be used for Enter action on disclaimer. Path is relative to /files/, so the image need to be in your /files directory. Default is "enter.png"'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['img']['disclaimer_enter_img_alt'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer enter image alt/title text'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_enter_img_alt', 'ENTER'),
    '#description' => t('Set the image alt/title text to be used for Enter action on disclaimer. Default is "Enter".'),
  );

  $form['disclaimer_action_style']['disclaimer_action']['img']['disclaimer_exit_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Disclaimer exit image name'),
    '#size' => 20,
    '#default_value' => variable_get('disclaimer_exit_img', 'exit.png'),
    '#description' => t('Set the image name to be used for Exit action on disclaimer. Path is relative to /files/, so the image need to be in your /files directory. Default is "exit.png"'),
  );
  $form['disclaimer_action_style']['disclaimer_action']['img']['disclaimer_exit_img_alt'] = array(
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
  $form['disclaimer_advanced']['disclaimer_location_js'] = array(
    '#type' => 'select',
    '#title' => t('Javascript'),
    '#default_value' => variable_get('disclaimer_location_js', 'header'),
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer'),
    ),
    '#description' => t('Where the javascript is loaded. Header is default, but could be useful to load on footer for conflict with other js. Note: this is only for disclaimer javascript and not jquery.nyroModal.js'),
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
    return system_settings_form($form);
}

/**
 * Validation function for the general configuration form.
 *
 * Ensure that numeric value are numeric... and set input format
 */
function disclaimer_admin_settings_validate($form, &$form_state) {
  $height = $form_state['values']['disclaimer_height'];
  $width = $form_state['values']['disclaimer_width'];
  $padding = $form_state['values']['disclaimer_padding'];
  $bgcolor = $form_state['values']['disclaimer_bgcolor'];
  $age_limit = $form_state['values']['disclaimer_age_limit'];
  $overlay = $form_state['values']['disclaimer_jqmodal_overlay'];

  if ($form_state['values']['disclaimer_input_format']) {
    variable_set('disclaimer_content_format', $form_state['values']['disclaimer_input_format']);
  }
  if ($form_state['values']['disclaimer_footer_input_format']) {
    variable_set('disclaimer_content_footer_format', $form_state['values']['disclaimer_footer_input_format']);
  }
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
}

