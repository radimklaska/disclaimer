// $Id$
/**
 *  @name disclaimer module js file
 */
function WriteCookie(nom, valeur){
  var cookie_path = Drupal.settings.disclaimer.cookie_path;
  var cookie_expires = Drupal.settings.disclaimer.cookie_expires;
  var cookie_domain = Drupal.settings.disclaimer.cookie_domain;
  var argv = WriteCookie.arguments;
  var argc = WriteCookie.arguments.length;
  var expires = (argc > 2) ? argv[2] : null;
  var secure = (argc > 5) ? argv[5] : false;
  if (cookie_path) {
    var path = cookie_path;
  } else {
    var path = (argc > 3) ? argv[3] : null;
  }
  if (cookie_domain) {
    var domain = cookie_domain;
  } else {
    var domain = (argc > 4) ? argv[4] : null;
  }
  if (cookie_expires) {
    var expires = cookie_expires;
  } else {
    var expires = (argc > 4) ? argv[4] : null;
  }
  document.cookie = nom + "=" + escape(valeur) +
  ((expires==null) ? "" : ("; expires=" + expires.toGMTString())) +
  ((path==null) ? "" : ("; path=" + path)) +
  ((domain==null) ? "" : ("; domain=" + domain)) +
  ((secure==true) ? "; secure" : "");
}

function CheckAge(cookie, limit, exit_url, modal) {
  var now = new Date();
  var date = now.getDate();
  var month = now.getMonth() + 1;
  var year = now.getFullYear();
  var optmonth = jQuery("#edit-disclaimer-age-month option:selected").val();
  var optday = jQuery("#edit-disclaimer-age-day option:selected").val();
  var optyear = jQuery("#edit-disclaimer-age-year option:selected").val();
  var age = year-optyear;
  if (optmonth>month) {age--;} else {if(optmonth==month && optday>=date) {age--;}}
  if (optyear==year) {
    alert(Drupal.t("You must enter the year you were born in."));return false;
  } else if (age<limit) {
    alert(Drupal.t("Sorry, you are Under age limit and are prohibited from entering this site!"));
    location.replace(exit_url);return false;
  } else {
    // age limit ok, close modal and set cookie
    WriteCookie(cookie);
    if (modal=='nyroModal'){jQuery.nyroModalRemove();}else{jQuery('#disclaimer').jqmHide();}
    return true;
  }
}