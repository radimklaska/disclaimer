<?php
// $Id$

/**
 * @file
 * Template file for displaying the disclaimer content.
 * It displays it without any sidebars, header or footer adjust to your need.
 * Created from page.tpl.php form GARLAND theme. Create your own one from your 
 * page.tpl.php file in your theme directory
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title ?></title>
    <style type="text/css" media="print">@import "<?php print base_path() . path_to_theme() ?>/print.css";</style>
  </head>
  <body>

    <div id="wrapper" style="background: transparent;">
      <div id="container" class="clear-block">

        <div id="center">
          <div id="squeeze">
            <div class="right-corner">
              <div class="left-corner">

                <?php print $content ?>

              </div><!-- left-corner -->
            </div><!-- right-corner -->
          </div><!-- squeeze -->
        </div><!-- center -->
      </div> <!-- close container -->
    </div> <!-- close wrapper -->
  </body>
</html>
