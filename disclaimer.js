(function ($, Drupal) {

  /**
   * Check cookie to display disclaimer
   */
  if ($.cookie("disclaimerShow") == null) {
    /**
     * Colorbox trigger, does not work (loop) if attached on behaviors
     * so we use old document ready.
     */
    $(document).ready(function(){
      // prevent colorbox error
      if (!$.isFunction($.colorbox)) {
        return;
      }
      // launch colorbox
      $.colorbox({
        html:Drupal.settings.disclaimer.content,
        width:Drupal.settings.disclaimer.width,
        height:Drupal.settings.disclaimer.height,
        initialWidth:Drupal.settings.disclaimer.initialwidth,
        initialHeight:Drupal.settings.disclaimer.initialheight,
        // no esc or close on click
        overlayClose:false,
        escKey:false,
        // remove close button
        onLoad:function(){$('#cboxClose').remove();}
      });
    });

    /**
     * Classic module behavior, handle enter button action
     */
    Drupal.behaviors.disclaimer = {
      attach: function (context, settings) {
        // action on enter button
        $('#disclaimer_enter', context).click(function () {
          var check = true;
          // age form is set
          if (settings.disclaimer.ageform == 1) {
            var check = Drupal.checkAge();
          }
          // everything good, add cookie and close colorbox
          if (check) {
            $.cookie(settings.disclaimer.cookie_name, '1', { path: settings.disclaimer.cookie_path });
            $.colorbox.remove();
          }
        });
      },
    };
  }

  /**
   * Control age limit.
   */
  Drupal.checkAge = function () {
    var now = new Date();
    var date = now.getDate();
    var month = now.getMonth() + 1;
    var year = now.getFullYear();
    var optmonth = jQuery("#edit-disclaimer-age-month option:selected").val();
    var optday = jQuery("#edit-disclaimer-age-day option:selected").val();
    var optyear = jQuery("#edit-disclaimer-age-year option:selected").val();
    var age = year - optyear;
    if (optmonth > month) {age--;} else {if(optmonth == month && optday >= date) {age--;}}
    // if current year, form not set
    if (optyear == year) {
      alert(Drupal.t("You must enter the year you were born in."));
      return false;
    // if under age, alert and redirect !
    } else if (age < Drupal.settings.disclaimer.limit) {
      alert(Drupal.t("Sorry, you are Under age limit and are prohibited from entering this site!"));
      location.replace(Drupal.settings.disclaimer.exiturl);
      return false;
    } else {
      // age limit ok
      return true;
    }
  }

}(jQuery, Drupal));
