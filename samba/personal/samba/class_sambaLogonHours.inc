<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2003-2010  Cajus Pollmeier
  Copyright (C) 2011-2016  FusionDirectory

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/

class sambaLogonHours
{
  var $sambaLogonHours = "";
  var $Matrix          = array();
  var $timezone        = 0;
  var $acl;

  function __construct ($slh, $acl = 'rw')
  {
    $this->sambaLogonHours  = $slh;
    $this->acl              = $acl;

    /* Get default timezone */
    $zone = timezone::get_default_timezone();
    $this->timezone = $zone['value'];

    /* Convert to bin */
    $tmp = '';
    for ($i = 0; $i < strlen($slh); $i++) {
      $tmp .= str_pad(base_convert($slh[$i], 16, 2), 4, '0', STR_PAD_LEFT);
    }
    $slh = $tmp;

    /* Set to default if value is empty or seams to be broken */
    if (strlen($slh) != 168) {
      $slh = str_pad('', 168, "1");
    }

    /* Rework string, because it was stored in little endian */
    $new = '';
    for ($i = 0; $i < 21; $i++) {
      $part         = strrev(substr($slh, $i * 8, 8));
      $byte['hi']   = substr($part, 0, 4);
      $byte['low']  = substr($part, 4, 4);
      $new .= $byte['hi'].$byte['low'];
    }

    /* Shift string to match given timezone settings */
    $shift_by = (168 + $this->timezone) % 168;
    for ($i = 0; $i < $shift_by; $i++) {
      $new = $new[167].$new;
      $new = substr($new, 0, 168);
    }

    /* Create matrix */
    $this->Matrix = array();
    for ($day = 0; $day < 7; $day++) {
      for ($hour  = 0; $hour < 24; $hour++) {
        $bitpos  = ($day * 24 + $hour) % 168;
        $this->Matrix[$day][$hour] = $new[$bitpos];
      }
    }
  }

  function execute()
  {
    $week_days = msgPool::weekdays();

    $smarty = get_smarty();
    $smarty->assign("acl", preg_match("/w/i", $this->acl));

    $hours = array();
    for ($i = 0; $i < 24; $i++) {
      $hours[$i] = $i;
    }

    $smarty->assign("Matrix", $this->Matrix);
    $smarty->assign("Days",   $week_days);
    $smarty->assign("Hours",  $hours);
    return $smarty->fetch(get_template_path("sambaLogonHours.tpl", TRUE, dirname(__FILE__)));
  }

  function save_object()
  {
    /* Check if dialog was opened and if there were any changes */
    if (isset($_POST['sambaLogonHoursPosted'])) {
      foreach ($this->Matrix as $day_key => $days) {
        foreach ($days as $hour_key => $hour) {
          if (isset($_POST['day_'.$day_key.'_'.$hour_key])) {
            $this->Matrix[$day_key][$hour_key] = 1;
          } else {
            $this->Matrix[$day_key][$hour_key] = 0;
          }
        }
      }
    }
  }

  function save()
  {
    /* Convert Matrix to Hex */
    $slh = "";
    foreach ($this->Matrix as $days) {
      foreach ($days as $hour) {
        $slh .= $hour;
      }
    }

    /* Shift string to match given timezone settings */
    $shift_by = (168 + ($this->timezone * (-1))) % 168;
    for ($i = 0; $i < $shift_by; $i++) {
      $slh = $slh[167].$slh;
      $slh = substr($slh, 0, 168);
    }

    /* Rework string, because it was stored in little endian */
    $new = '';
    for ($i = 0; $i < 21; $i++) {
      $part         = strrev(substr($slh, $i * 8, 8));
      $byte['hi']   = substr($part, 0, 4);
      $byte['low']  = substr($part, 4, 4);
      $new .= $byte['hi'].$byte['low'];
    }

    /* Convert to bin */
    $tmp = '';
    for ($i = 0; $i < 21; $i++) {
      $tmp .= str_pad(base_convert(substr($new, $i * 8, 8), 2, 16), 2, '0', STR_PAD_LEFT);
    }

    /* Create uppercase HEX chars */
    $ret = strtoupper( $tmp);

    /* All hours and days selected */
    if (substr_count($ret, 'F') == 42) {
      $ret = "";
    }

    return $ret;
  }
}

class SambaLogonHoursDialog
{
  private $dialog;
  private $sambaAttribute;

  function __construct ($simplePlugin, &$sambaAttribute)
  {
    $this->sambaAttribute = $sambaAttribute;
    $this->dialog         = new sambaLogonHours(
      $sambaAttribute->getValue(),
      $simplePlugin->aclGetPermissions('sambaLogonHours')
    );
  }

  function execute ()
  {
    if (isset($_POST['cancel_logonHours'])) {
      return FALSE;
    }
    $this->dialog->save_object();
    if (isset($_POST['save_logonHours'])) {
      $this->sambaAttribute->setValue($this->dialog->save());
      return FALSE;
    }
    return $this->dialog->execute();
  }
}
?>
