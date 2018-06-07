<?php

/**
 * Returns $form for course booking detail
 *
 * @param int $bookingid
 * @return string
 */
function ecl_show_course_booking_details($bookingid) {

  $sql = "SELECT firstname, lastname, date_of_birth, email, telephone, address1, address2, town_city,
                 region, postcode, country, created, course_interested, course_date, total_booking_fee
            FROM ecl_bookings
           WHERE booking_id = :bookingid";
  $query = db_query($sql, array('bookingid' => $bookingid));
  $booking = $query->fetchAssoc();

  $markup  = '';
  $markup .= '<div class="table-responsive">';
  $markup .= '<table class="table table-bordered table-striped">';
  $markup .= '<thead>';
  $markup .= '<tr class="info">';
  $markup .= '<th colspan="4" width="100%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Student</strong></p>';
  $markup .= '</th>';
  $markup .= '</tr>';
  $markup .= '</thead>';
  $markup .= '<tbody>';
  $markup .= '<tr>';
  $markup .= '<td width="18%">';
  $markup .= '<p class="text-left"><strong>Name:</strong></p>';
  $markup .= '</td>';
  $markup .= '<td width="32%">';
  $markup .= '<p class="text-left">' . $booking['firstname'] . ' ' . $booking['lastname'] . '</p>';
  $markup .= '</td>';
  $markup .= '<td width="18%" rowspan="4" style="vertical-align:top;">';
  $markup .= '<p class="text-left"><strong>Address:</strong></p>';
  $markup .= '</td>';
  $markup .= '<td width="32%" rowspan="4" style="vertical-align:top;">';
  $address = $booking['address1'] . '<br/>';
  if ($booking['address2'] != '') {
    $address .= $booking['address2'] . '<br/>';
  }
  $address .= $booking['town_city'] . '<br/>' . $booking['region'] . '<br/>' . $booking['postcode'] . '<br/>' . $booking['country'];
  $markup .= '<p class="text-left">' . $address . '</p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '<tr>';
  $markup .= '<td width="18%">';
  $markup .= '<p class="text-left"><strong>Date of birth:</strong></p>';
  $markup .= '</td>';
  $markup .= '<td width="32%">';
  $markup .= '<p class="text-left">' . date('F jS, Y', strtotime($booking['date_of_birth'])) . '</p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '<tr>';
  $markup .= '<td width="18%">';
  $markup .= '<p class="text-left"><strong>Email:</strong></p>';
  $markup .= '</td>';
  $markup .= '<td width="32%">';
  $markup .= '<p class="text-left"><a href="mailto:' . $booking['email'] . '">' . $booking['email'] . '</a></p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '<tr>';
  $markup .= '<td width="18%">';
  $markup .= '<p class="text-left"><strong>Telephone:</strong></p>';
  $markup .= '</td>';
  $markup .= '<td width="32%">';
  $markup .= '<p class="text-left">' . $booking['telephone'] . '</p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '</tbody>';
  $markup .= '</table>';
  $markup .= '</div>';
  $markup .= '<div class="table-responsive">';
  $markup .= '<table class="table table-bordered table-striped">';
  $markup .= '<thead>';
  $markup .= '<tr class="info">';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Course</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Course Date</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p class="text-left" style="margin-bottom: 0;"><strong>Booking Made</strong></p>';
  $markup .= '</th>';
  $markup .= '<th width="25%">';
  $markup .= '<p style="margin-bottom: 0;"><strong>Price</strong></p>';
  $markup .= '</th>';
  $markup .= '</tr>';
  $markup .= '</thead>';
  $markup .= '<tbody>';
  $markup .= '<tr>';
  $markup .= '<td>';
  $markup .= '<p class="text-left">' . $booking['course_interested'] . '</p>';
  $markup .= '</td>';
  $markup .= '<td>';
  $markup .= '<p class="text-left">' . $booking['course_date'] . '</p>';
  $markup .= '</td>';
  $markup .= '<td>';
  $markup .= '<p class="text-left">' . date('F jS, Y g:i A', $booking['created']) . '</p>';
  $markup .= '</td>';
  $markup .= '<td class="text-left">';
  $markup .= '<p>&pound;' . $booking['total_booking_fee'] . '</p>';
  $markup .= '</td>';
  $markup .= '</tr>';
  $markup .= '</tbody>';
  $markup .= '</table>';
  $markup .= '</div>';

  $payments = db_query("SELECT * FROM {ecl_payments} WHERE booking_id = :bookingid", array('bookingid' => $bookingid))->fetchAll();

  if (!empty($payments)) {
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
  }

  foreach ($payments as $payment) {
    $worldpaydata = json_decode($payment->worldpay_data);
    $markup .= '<tr>';
    $markup .= '<td>';
    $markup .= $payment->payment_id;
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

  if (!empty($payments)) {
    $markup .= '</tbody>';
    $markup .= '</table>';
    $markup .= '</div>';
  }

  $form['bookingdetails'] = array(
    '#markup' => $markup,
  );

  $form['back_button'] = array(
    '#markup' => "<input type='button' class='form-submit' value='Back' onclick=\"window.location='/admin/settings/ecl/coursebookings';\" />",
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

/**
 * Returns start date options
 *
 * @return array
 */
function ecl_course_date_options() {

  $defaultplaces = variable_get('places_on_courses', '20');

  $sql = "SELECT course_date, COUNT(course_date) AS confirmed_bookings
            FROM ecl_bookings
           WHERE status_id = :status_id
        GROUP BY course_date";
  $query = db_query($sql, array('status_id' => 1));
  $bookings = $query->fetchAll();

  $availability = array();
  foreach ($bookings as $booking) {
    $availability[$booking->course_date] = $defaultplaces - $booking->confirmed_bookings;
  }

  $nextwednesday = strtotime('next wednesday');
  $nextthursday = strtotime('next thursday');
  $nextfriday = strtotime('next friday');

  $nextdate = min(array($nextwednesday, $nextthursday, $nextfriday));

  $firstdate = mktime(0, 0, 0, date('n', $nextdate), date('j', $nextdate), date('Y', $nextdate));
  $lastdate = strtotime('+26 week', $firstdate);

  $dateoptions = array();
  $date = $firstdate;

  while ($date < $lastdate) {
    if (in_array(date('w', $date), array(3, 4, 5))) {
      $formatteddate = date('l, F jS, Y', $date);
      $placesavailable = (isset($availability[$formatteddate])) ? $availability[$formatteddate] : $defaultplaces;
      $dateoptions[$formatteddate] = $formatteddate . " ({$placesavailable} places available)";
    }
    $date = strtotime('+1 day', $date);
  }

  return $dateoptions;
}

/**
 * Returns HTML to populate the payment confirmation block.
 *
 * @param int $bookingid The booking Id
 * @return string
 */
function ecl_payment_confirmation_content($bookingid) {

  if (!empty($bookingid)) {
    $markup = "<p>Your payment for booking Id {$bookingid} has been confirmed.</p>";
  } else {
    $markup = "<p>No booking Id found.</p>";
  }

  return $markup;
}
