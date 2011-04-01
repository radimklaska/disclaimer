/**
  *  @name    disclaimer module js file
  */

if (ReadCookie("disclaimerShow") == null) {
  $(document).ready(function(){
    var modal = Drupal.settings.disclaimer.modal;
    if (modal == 'colorbox') {
      $.colorbox({
        href:"/disclaimer",
        overlayClose:false,
        escKey:false,
        onLoad:function(){$('#cboxClose').remove();},
        // settings
        width: Drupal.settings.disclaimer.width,
        height: Drupal.settings.disclaimer.height,
        speed: Drupal.settings.disclaimer.speed,
        opacity: Drupal.settings.disclaimer.opacity,
        transition: Drupal.settings.disclaimer.transition,
      });
    } else if (modal == 'thickbox') {
      tb_show('', "/disclaimer?modal=true&height=" + Drupal.settings.disclaimer.height + "&width=" + Drupal.settings.disclaimer.width);
    }
  });
}

function getCookieVal(offset) {
  var endstr  =document.cookie.indexOf (";", offset);
  if (endstr == -1) endstr = document.cookie.length;
  return unescape(document.cookie.substring(offset, endstr));
}

function ReadCookie(nom) {
  var arg = nom+"=";
  var alen = arg.length;
  var clen = document.cookie.length;
  var i = 0;
  while (i < clen) {
    var j = i + alen;
    if (document.cookie.substring(i, j) == arg) return getCookieVal(j);
    i = document.cookie.indexOf(" ",i) + 1;
    if (i == 0) break;
  }
  return null;
}

function WriteCookie(nom, valeur){
  var argv = WriteCookie.arguments;
  var argc = WriteCookie.arguments.length;
  var expires = (argc > 2) ? argv[2] : null;
  var secure = (argc > 5) ? argv[5] : false;
  //var path=(argc > 3) ? argv[3] : null;
  //var domain=(argc > 4) ? argv[4] : null;
  var path = Drupal.settings.disclaimer.cookie_path;
  var domain = Drupal.settings.disclaimer.cookie_domain;
  
  document.cookie = nom + "=" + escape(valeur) +
  ((expires==null) ? "" : ("; expires=" + expires.toGMTString())) +
  ((path==null) ? "" : ("; path=" + path)) +
  ((domain==null) ? "" : ("; domain="+domain))+
  ((secure==true) ? "; secure" : "");
}

function CheckAge(cookie, limit, exit_url, modal) {
  var now = new Date();
  var date = now.getDate();
  var month = now.getMonth() + 1;
  var year = now.getFullYear();
  var optmonth = $("#edit-disclaimer-age-month option:selected").val();
  var optday = $("#edit-disclaimer-age-day option:selected").val();
  var optyear = $("#edit-disclaimer-age-year option:selected").val();
  var age = year - optyear;
  if (optmonth > month) {
    age--;
  } else {
    if(optmonth == month && optday >= date) {
      age--;
    }
  }
  if (optyear == year) {
    alert(Drupal.t("You must enter the year you were born in."));
    return false;
  } else if (age < limit) {
    alert(Drupal.t("Sorry, you are Under age limit and are prohibited from entering this site!"));
    location.replace(exit_url);
    return false;
  } else {
    // age limit ok, close modal and set cookie
    WriteCookie(cookie);
    switch (modal) {
     case 'thickbox':
      tb_remove();
      break;
     case 'colorbox':
      $.colorbox.remove();
      break;
    }
    return true;
  }
}
