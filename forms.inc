<?php

/**
 * Make course payment form
 *
 * @param array $form
 * @param array $form_state
 * @param int $bookingid
 * @return array
 */
function ecl_make_course_booking_payment_form($form, &$form_state, $bookingid) {

  $sql = "SELECT eb.firstname, eb.lastname, eb.created, eb.course_interested, eb.course_date, eb.total_booking_fee, eb.outstanding_fee
            FROM ecl_bookings eb
           WHERE booking_id = :bookingid";
  $query = db_query($sql, array('bookingid' => $bookingid));
  $booking = $query->fetchAssoc();

  $markup  = '<p>Make a payment for booking ID ' . $bookingid . '</p>';
  $markup .= '<ul>';
  $markup .= "<li>Student name: {$booking['firstname']} {$booking['lastname']}</li>";
  $markup .= "<li>Booking made: " . date('F j, Y', $booking['created']) . "</li>";
  $markup .= "<li>Course: {$booking['course_interested']}</li>";
  $markup .= "<li>Course date: {$booking['course_date']}</li>";
  $markup .= "<li>Course fee: £{$booking['total_booking_fee']}</li>";
  $markup .= "<li>Outstanding fee: £{$booking['outstanding_fee']}</li>";
  $markup .= '</ul>';

  $form['bookingdetails'] = array(
    '#markup' => $markup,
  );

  $form['booking_id'] = array(
    '#value' => $bookingid,
    '#type' => 'hidden',
  );

  $form['payment_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment amount (£)'),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#value' => number_format($booking['outstanding_fee'], 0, '', ''),
  );

  $form['back_button'] = array(
    '#markup' => "<input type='button' class='form-submit' value='Back' onclick=\"window.location='/admin/settings/ecl/coursebookings';\" />",
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );

  return $form;
}

/**
 * Make course payment form validation
 *
 * @param array $form
 * @param array $form_state
 */
function ecl_make_course_booking_payment_form_validate($form, &$form_state) {

}

/**
 * Make course payment form submit
 *
 * @param array $form
 * @param array $form_state
 */
function ecl_make_course_booking_payment_form_submit($form, &$form_state) {

  $_SESSION['booking_id'] = $form_state['input']['booking_id'];
  $_SESSION['payment_amount'] = $form_state['input']['payment_amount'];

  if (variable_get('online_payment_functionality', 0) == 1) {
      $form_state['redirect'] = 'make-worldpay-payment';
  } else {
      $form_state['redirect'] = 'admin/settings/ecl/coursebookings';
  }
}

/**
 * Make custom payment to Worldpay form.
 *
 * @param array $form
 * @param array $form_state
 * @return string
 */
function ecl_make_worldpay_payment_form($form, &$form_state) {
  global $base_url;
  global $base_path;

  $booking = db_select('ecl_bookings', 'b')
    ->fields('b')
    ->condition('booking_id', $_SESSION['booking_id'], '=')
    ->execute()
    ->fetchAssoc();

  if (variable_get('worldpay_mode') == 1) {
    $testmode = 0;
  } else {
    $testmode = 100;
  }

  $amount = $_SESSION['payment_amount'];
  $description = "Payment in regard to English College London Booking Id {$_SESSION['booking_id']}";

  $form['instId'] = array(
    '#value' => ecl_get_worldpay_instid(),
    '#type' => 'hidden',
  );

  $form['testMode'] = array(
    '#value' => $testmode,
    '#type' => 'hidden',
  );
  $form['cartId'] = array(
    '#value' => $booking['booking_id'],
    '#type' => 'hidden',
  );

  $form['MC_callback'] = array(
    '#value' => $base_url . $base_path . 'sites/all/modules/custom/ecl/confirm_wp_payment.php',
    '#type' => 'hidden',
  );

  $form['amount'] = array(
    '#value' => $amount,
    '#type' => 'hidden'
  );

  $form['currency'] = array(
    '#value' => 'GBP',
    '#type' => 'hidden',
  );

  $form['name'] = array(
    '#value' => $booking['firstname'] . ' ' . $booking['lastname'],
    '#type' => 'hidden',
  );

  $form['desc'] = array(
    '#value' => $description,
    '#type' => 'hidden',
  );

  $form['confirm_url'] = array(
    '#value' => $base_url . $base_path . 'payment-response',
    '#type' => 'hidden',
  );

  $form['cancel_url'] = array(
    '#value' => $base_url . $base_path . 'payment-cancelled',
    '#type' => 'hidden',
  );

  $form['status_url'] = array(
    '#value' => 'ed@theteflacademy.com', # <-- not sure
    '#type' => 'hidden',
  );

  $form['address1'] = array(
    '#value' => $booking['address1'],
    '#type' => 'hidden',
  );

  $form['address2'] = array(
    '#value' => $booking['address2'],
    '#type' => 'hidden',
  );

  $form['town'] = array(
    '#value' => $booking['town_city'],
    '#type' => 'hidden',
  );

  $form['region'] = array(
   '#value' => $booking['region'],
   '#type' => 'hidden',
  );

  $form['postcode'] = array(
    '#value' => $booking['postcode'],
    '#type' => 'hidden',
  );

  $form['country'] = array(
    '#value' => $booking['country'],
    '#type' => 'hidden',
  );

  $form['tel'] = array(
    '#value' => $booking['telephone'],
    '#type' => 'hidden',
  );

  $form['email'] = array(
    '#value' => $booking['email'],
    '#type' => 'hidden',
  );

  return $form;
}
