(function ($) {
  Drupal.behaviors.ecl = {
    attach: function (context, settings) {
        $('#ecl-worldpay-form').delay(5000).submit();
    }
  }
})(jQuery);