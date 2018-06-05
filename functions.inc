<?php

/**
 * Returns $form for course booking detail
 *
 * @param int $bookingid
 * @return string
 */
function ecl_show_course_booking_details($bookingid) {

  $sql = "SELECT eb.course_interested, eb.course_date, eb.total_booking_fee
            FROM ecl_bookings eb
           WHERE eb.booking_id = :bookingid";
  $query = db_query($sql, array('bookingid' => $bookingid));
  $booking = $query->fetchAssoc();

  $markup  = '<div class="table-responsive">';
  $markup .= '<table class="table table-bordered table-striped">';
  $markup .= '<thead>';
  $markup .= '<tr class="info">';
  $markup .= '<th width="65%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Course</strong></p>';
  $markup .= '</th>';
  $markup .= '<th class="price" style="text-align: right;" width="35%">';
  $markup .= '<p style="margin-bottom: 0;"><strong>Price</strong></p>';
  $markup .= '</th>';
  $markup .= '</tr>';
  $markup .= '</thead>';
  $markup .= '<tbody>';
  $markup .= '<tr>';
  $markup .= '<td>';
  $markup .= '<p class="text-left">' . $booking['course_interested'] . ' - ' . $booking['course_date'] . '</p>';
  $markup .= '</td>';
  $markup .= '<td class="price" style="text-align: right;">';
  $markup .= '<p>&pound;' . $booking['total_booking_fee'] . '</p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '</tbody>';
  $markup .= '</table>';
  $markup .= '</div>';

  $payments = db_query("SELECT * FROM {ecl_payments} WHERE booking_id = :bookingid", array('bookingid' => $bookingid));

  $markup .= '<div class="table-responsive">';
  $markup .= '<table class="table table-bordered table-striped">';
  $markup .= '<thead>';
  $markup .= '<tr class="info">';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Payment ID</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Payment Date/Time</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>WorldPay Transaction ID</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Payment Amount</strong></p>';
  $markup .= '</th>';
  $markup .= '</tr>';
  $markup .= '</thead>';
  $markup .= '<tbody>';

  while ($payment = $payments->fetchAssoc()) {
    $worldpaydata = json_decode($payment['worldpay_data']);
    $markup .= '<tr>';
    $markup .= '<td>';
    $markup .= $payment['payment_id'];
    $markup .= '</td>';
    $markup .= '<td>';
    $markup .= date('M j, Y g:i:s A', $worldpaydata->transTime / 1000);
    $markup .= '</td>';
    $markup .= '<td>';
    $markup .= $worldpaydata->transId;
    $markup .= '</td>';
    $markup .= '<td>';
    $markup .= $worldpaydata->authAmountString;
    $markup .= '</td>';
    $markup .= '</tr>';
  }

  $markup .= '</tbody>';
  $markup .= '</table>';
  $markup .= '</div>';

  $form['bookingdetails'] = array(
    '#markup' => $markup,
  );

  return $form;
}

/**
 * Returns 'Make Course Booking Payment' form
 *
 * @param int $bookingid
 * @return drupal form
 */
function ecl_make_course_booking_payment($bookingid) {

  return drupal_get_form('ecl_make_course_booking_payment_form', $bookingid);
}