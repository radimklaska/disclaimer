<?php

namespace Drupal\sentry\Plugin\Block;

use Drupal\Component\Utility\Html;
use Drupal\Component\Utility\UrlHelper;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Cache\Cache;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;

/**
 * Provides a 'SentryBlock' block.
 *
 * @Block(
 *  id = "sentry_block",
 *  admin_label = @Translation("Sentry block"),
 * )
 */
class SentryBlock extends BlockBase
{
    /**
     * {@inheritdoc}
     */
    public function getCacheContexts()
    {
        return Cache::mergeContexts(parent::getCacheContexts(), ['cookies:sentry_' . $this->configuration['machine_name'], 'url.path']);
    }

    /**
     * {@inheritdoc}
     */
    public function getCacheMaxAge()
    {
        return $this->configuration['max_age'];
    }

    /**
     * {@inheritdoc}
     */
    public function defaultConfiguration()
    {
        return [
                'machine_name' => $this->t(''),
                'challenge' => $this->t(''),
                'disclaimer' => $this->t(''),
                'redirect' => $this->t('/'),
                'max_age' => $this->t('86400'),
            ] + parent::defaultConfiguration();
    }

    /**
     * {@inheritdoc}
     */
    public function blockForm($form, FormStateInterface $form_state)
    {
//        $current_user = \Drupal::currentUser();
//        $filter_formats = filter_formats($current_user);
//        $filter_formats_form = [];
//        foreach ($filter_formats as $filter_format) {
//            $filter_formats_form[$filter_format->id()] = $filter_format->label();
//        }

        $form['machine_name'] = [
            '#type' => 'textfield',
            '#title' => $this->t('Machine name'),
            '#description' => $this->t('Used as ID in cookies.'),
            '#default_value' => $this->configuration['machine_name'],
            '#maxlength' => 64,
            '#size' => 64,
            '#weight' => '10',
        ];
        $form['challenge'] = [
//            '#type' => 'text_format',
//            '#format' => $filter_formats_form,
            '#type' => 'textarea',
            '#title' => $this->t('Challenge'),
            '#description' => $this->t('The question the user must confirm. "Do you agree?" type of question. "Yes" = User stays on requested page. "No" = User is redirected to <em>Redirect</em> url specified below.'),
            '#default_value' => $this->configuration['challenge'],
            '#weight' => '20',
        ];
        $form['redirect'] = [
            '#type' => 'textfield',
            '#title' => $this->t('Redirect'),
            '#description' => $this->t('The URL a rejected user is sent to. eg. /content-for-unconfirmed-users. (relative, absolute, &lt;front&gt;)'),
            '#default_value' => $this->configuration['redirect'],
            '#maxlength' => 256,
            '#size' => 64,
            '#weight' => '30',
        ];
        $form['disclaimer'] = [
            '#type' => 'textarea',
            '#title' => $this->t('Disclaimer'),
            '#description' => $this->t('The text displayed to the user on a protected page when the user has JS turned off. (No popup wich challenge is available.)'),
            '#default_value' => $this->configuration['disclaimer'],
            '#weight' => '40',
        ];
        $form['max_age'] = [
            '#type' => 'textfield',
            '#title' => $this->t('Max-age'),
            '#description' => $this->t('The time in seconds the user is confirmed. Set to 0 for no expiry. (86400 seconds = 24 hours)'),
            '#default_value' => $this->configuration['max_age'],
            '#maxlength' => 64,
            '#size' => 64,
            '#weight' => '50',
        ];

        return $form;
    }

    /**
     * {@inheritdoc}
     */
    public function blockSubmit($form, FormStateInterface $form_state)
    {
        $this->configuration['machine_name'] = $form_state->getValue('machine_name');
        $this->configuration['challenge'] = $form_state->getValue('challenge');
        $this->configuration['disclaimer'] = $form_state->getValue('disclaimer');
        $this->configuration['redirect'] = $form_state->getValue('redirect');
        $this->configuration['max_age'] = $form_state->getValue('max_age');
    }

    /**
     * {@inheritdoc}
     */
    public function blockValidate($form, FormStateInterface $form_state)
    {
        $url_object = \Drupal::service('path.validator')->getUrlIfValid($form_state->getValue('redirect'));

        if (!$url_object) {
            $form_state->setErrorByName('redirect', $this->t('Redirect URL must be valid path.'));
        }
        if (!preg_match('/^[a-zA-Z0-9_]+$/', $form_state->getValue('machine_name'))) {
            $form_state->setErrorByName('machine_name', $this->t('Machine name must contain only letters, numbers and underscores.'));
        }
        if (!preg_match('/^[0-9]+$/', $form_state->getValue('max_age'))) {
            $form_state->setErrorByName('max_age', $this->t('Max-age must be integer.'));
        }
    }

    /**
     * {@inheritdoc}
     */
    public function build()
    {
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
            'challenge' => Html::escape($this->configuration['challenge']),
            'redirect' => \Drupal::service('path.validator')->getUrlIfValid($this->configuration['redirect'])->toString(),
            'max_age' => Html::escape($this->configuration['max_age']),
        ];

        // Render disclaimer.
        $build['sentry_block_disclaimer'] = [
            '#type' => 'container',
            '#attributes' => [
                'class' => [
                    'disclaimer',
                ],
            ],
            '#markup' => Html::escape($this->configuration['disclaimer']),
        ];

        // Render popup HTML.
        $build['sentry_block_challenge'] = [
            '#type' => 'container',
            '#attributes' => [
                'class' => [
                    'challenge',
                    'hidden',
                ],
                'title' => [
                    Html::escape($this->label()),
                ],
            ],
            '#markup' => Html::escape($this->configuration['challenge']),
        ];

        return $build;
    }

}
