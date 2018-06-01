<?php

if (!$response = $_REQUEST) {
  die;
}

define('DRUPAL_ROOT', dirname(__FILE__) . '/../../../../..');
require_once(DRUPAL_ROOT . '/includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if ($response['transStatus'] == 'Y') {

  // Retrieve the users Drupal account
  $account = user_load_by_mail($response['email']);

  // Write the WorldPay response to the database
  $fields = array(
    'worldpay_data' => json_encode($response),
    'booking_id' => $response['cartId'],
    'user_id' => $account->uid,
  );
  $paymentid = db_insert('ecl_payments')->fields($fields)->execute();

  // Look up the current outstanding balance on the booking
  $query = db_select('ecl_bookings', 'b')
    ->fields('b', array('outstanding_fee'))
    ->condition('b.booking_id', $response['cartId'])
    ->execute();
  $result = $query->fetchObject();
  $outstandingfee = $result->outstanding_fee;

  // Set the booking new outstanding amount
  $fields = array(
    'outstanding_fee' => $outstandingfee - $response['authAmount']
  );

  // Set the booking status to confirmed if payment covers outstanding
  if ($response['authAmount'] >= $outstandingfee) {
    $fields['status_id'] = 1;
  }

  $bookingid = db_update('ecl_bookings')->fields($fields)
          ->condition('booking_id', $response['cartId'], '=')->execute();

  // Send user an email
  $from = 'info@englishcollegelondon.co.uk';
  $to = $response['email'];
  $subject = 'Payment Received';
  $body = "Thanks, your payment for booking ID {$response['cartId']} has been received.";
  simple_mail_send($from, $to, $subject, $body);

  $url = 'payment-confirmation';
} else if ($response['rawAuthCode'] == 'C') {
  $url = 'payment-cancelled';
} else {
  $url = '';
}
?>
<meta http-equiv="refresh" content= "0;URL=<?php echo 'https://staging.englishcollegelondon.co.uk/' . $url; ?>" />
