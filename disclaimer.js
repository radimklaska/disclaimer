/**
  *  @name    disclaimer module js file
  */
function getCookieVal(offset) {
  var endstr=document.cookie.indexOf (";", offset);
  if (endstr==-1) endstr=document.cookie.length;
  return unescape(document.cookie.substring(offset, endstr));
}
function ReadCookie(nom) {
  var arg=nom+"=";
  var alen=arg.length;
  var clen=document.cookie.length;
  var i=0;
  while (i < clen) {
    var j=i+alen;
    if (document.cookie.substring(i, j)==arg) return getCookieVal(j);
    i=document.cookie.indexOf(" ",i)+1;
    if (i==0) break;
  }
  return null;
}
function CheckAge(cookie, limit, exit_url, modal) {
  var now=new Date();
  var date=now.getDate();
  var month=now.getMonth() + 1;
  var year=now.getFullYear();
  var optmonth=$("#edit-disclaimer-age-month option:selected").val();
  var optday=$("#edit-disclaimer-age-day option:selected").val();
  var optyear=$("#edit-disclaimer-age-year option:selected").val();
  var age=year-optyear;
  if (optmonth>month) {age--;} else {if(optmonth==month && optday>=date) {age--;}}
  if (optyear==year) {
    alert(Drupal.t("You must enter the year you were born in."));return false;
  } else if (age<limit) {
    alert(Drupal.t("Sorry, you are Under age limit and are prohibited from entering this site!"));
    location.replace(exit_url);return false;
  } else {
    // age limit ok, close modal and set cookie
    WriteCookie(cookie);
    switch (modal) {
     case 'nyroModal':
      $.nyroModalRemove();
     break;
     case 'jqModal':
      $('#disclaimer').jqmHide();
     break;
     case 'thickbox':
      tb_remove();
     break;
    }
    return true;
  }
}