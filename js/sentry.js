/**
 * @file
 * Attaches popup and show/hide functionality to disclaimer.
 */
(function ($) {
    'use strict';
    Drupal.behaviors.sentry = {
        attach: function (context, settings) {
            // Show/Hide based on Javascript availability.
            $('.sentryNoScript').hide();

            // Go trough all sentry block instances on thos page.
            Object.keys(drupalSettings.sentry.sentry).forEach(function (key) {
                    // Skip popup in case cookie says user already agreed.
                    if ($.cookie(key) != 1) {
                        // User did not agreed yet. Show popup.
                        $('.block-sentry-block.' + key + ' .sentry__challenge', context).dialog({
                                closeOnEscape: false,
                                open: function (event, ui) {
                                    $(".ui-dialog-titlebar-close", ui.dialog | ui).hide();
                                },
                                resizable: false,
                                height: "auto",
                                width: "40%",
                                modal: true,
                                buttons: {
                                    "Yes": {
                                        text: Drupal.t("Yes"),
                                        click: function () {
                                            $(this).dialog("close");
                                            var expire = new Date(new Date().getTime() + parseInt(drupalSettings.sentry.sentry[key].max_age) * 1000);
                                            $.cookie(key, 1, {expires: expire});
                                        }
                                    },
                                    "No": {
                                        text: Drupal.t("No"),
                                        click:

                                            function () {
                                                $(this).dialog("close");
                                                window.location.href = drupalSettings.sentry.sentry[key].redirect;
                                            }
                                    }
                                }
                            }
                        );
                    }
                }
            );
        }
    }
    ;
})
(jQuery);
