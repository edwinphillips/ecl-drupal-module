(function ($) {
  Drupal.behaviors.ecl = {
    attach: function (context, settings) {
        $('#ecl-make-worldpay-payment-form').delay(5000).submit();
    }
  }
})(jQuery);