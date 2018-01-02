<?php

namespace Drupal\sentry\Plugin\Block;

use Drupal\Component\Utility\Html;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Cache\Cache;
use Drupal\Core\Form\FormStateInterface;

/**
 * Provides a 'SentryBlock' block.
 *
 * @Block(
 *  id = "sentry_block",
 *  admin_label = @Translation("Sentry block"),
 * )
 */
class SentryBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function getCacheContexts() {
    return Cache::mergeContexts(parent::getCacheContexts(), [
      'cookies:sentry_' . $this->configuration['machine_name'],
      'url.path',
    ]);
  }

  /**
   * {@inheritdoc}
   */
  public function getCacheMaxAge() {
    return $this->configuration['max_age'];
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return [
      'machine_name' => 'sentry_block' . time(),
      'challenge' => [
        'format' => filter_fallback_format(),
        'value' => '',
      ],
      'disclaimer' => [
        'format' => filter_fallback_format(),
        'value' => '',
      ],
      'redirect' => '/',
      'max_age' => 86400,
    ] + parent::defaultConfiguration();
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['challenge'] = [
      '#type' => 'text_format',
      '#format' => $this->configuration['challenge']['format'],
      '#title' => $this->t('Challenge'),
      '#description' => $this->t('The question the user must confirm. "Do you agree?" type of question. "Yes" = User stays on requested page. "No" = User is redirected to <em>Redirect</em> url specified below.'),
      '#default_value' => $this->configuration['challenge']['value'],
      '#required' => TRUE,
      '#weight' => '20',
    ];
    $form['redirect'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Redirect'),
      '#description' => $this->t('The URL a rejected user is sent to. eg. /content-for-unconfirmed-users. (relative, absolute, &lt;front&gt;)'),
      '#default_value' => $this->configuration['redirect'],
      '#maxlength' => 256,
      '#size' => 64,
      '#required' => TRUE,
      '#weight' => '30',
    ];
    $form['disclaimer'] = [
      '#type' => 'text_format',
      '#format' => $this->configuration['disclaimer']['format'],
      '#title' => $this->t('Disclaimer'),
      '#description' => $this->t('The text displayed to the user on a protected page when the user has JS turned off. (No popup with challenge is available.)'),
      '#default_value' => $this->configuration['disclaimer']['value'],
      '#weight' => '40',
      '#required' => FALSE,
    ];
    $form['max_age'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Max-age'),
      '#description' => $this->t('The time in seconds the user is confirmed. Set to 0 for no expiry. (86400 seconds = 24 hours)'),
      '#default_value' => $this->configuration['max_age'],
      '#maxlength' => 64,
      '#size' => 64,
      '#required' => TRUE,
      '#weight' => '50',
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['machine_name'] = $form_state->getCompleteFormState()
      ->getValue('id');
    $this->configuration['challenge'] = $form_state->getValue('challenge');
    $this->configuration['disclaimer'] = $form_state->getValue('disclaimer');
    $this->configuration['redirect'] = $form_state->getValue('redirect');
    $this->configuration['max_age'] = $form_state->getValue('max_age');
  }

  /**
   * {@inheritdoc}
   */
  public function blockValidate($form, FormStateInterface $form_state) {
    $url_object = \Drupal::service('path.validator')
      ->getUrlIfValid($form_state->getValue('redirect'));

    if (!$url_object) {
      $form_state->setErrorByName('redirect', $this->t('Redirect URL must be valid path.'));
    }
    if (!preg_match('/^[0-9]+$/', $form_state->getValue('max_age'))) {
      $form_state->setErrorByName('max_age', $this->t('Max-age must be integer.'));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    // Identify block by class with machine name.
    $build = [
      '#type' => 'container',
      '#attributes' => [
        'class' => [
          'sentry_' . Html::escape($this->configuration['machine_name']),
          'sentryNoScript',
        ],
      ],
    ];

    // Include JS to handle popup and hiding.
    $build['#attached']['library'][] = 'sentry/sentry';
    // Pass settings to JS.
    $build['#attached']['drupalSettings']['sentry']['sentry']['sentry_' . Html::escape($this->configuration['machine_name'])] = [
      'redirect' => $this->configuration['redirect'],
      'max_age' => Html::escape($this->configuration['max_age']),
    ];

    // Render disclaimer.
    $build['sentry_block_disclaimer'] = [
      '#type' => 'container',
      '#attributes' => [
        'class' => [
          'sentry__disclaimer',
        ],
      ],
      '#markup' => check_markup($this->configuration['disclaimer']['value'], $this->configuration['disclaimer']['format']),
    ];

    // Render popup HTML.
    $build['sentry_block_challenge'] = [
      '#type' => 'container',
      '#attributes' => [
        'class' => [
          'sentry__challenge',
          'hidden',
        ],
        'title' => [
          Html::escape($this->label()),
        ],
      ],
      '#markup' => check_markup($this->configuration['challenge']['value'], $this->configuration['challenge']['format']),
    ];

    return $build;
  }

}
