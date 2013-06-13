<?php
/*
 * This code is part of FusionDirectory (http://www.fusiondirectory.org)
 * Copyright (C) 2013 FusionDirectory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Remove locks created by this plugin */
if ($remove_lock) {
  if (session::is_set('sogoManagement')) {
      $macl = session::get('sogoManagement');
      $macl->remove_lock();
  }
}

/* Remove this plugin from session */
if ($cleanup) {
  session::un_set('sogoManagement');
} else {

  /* Create usermanagement object on demand */
  if (!session::is_set('sogoManagement')) {
    $sogoManagement = new sogoManagement ($config, $ui);
    session::set('sogoManagement', $sogoManagement);
  }
  $sogoManagement = session::get('sogoManagement');
  $display = $sogoManagement->execute();

  /* Reset requested? */
  if (isset($_GET['reset']) && $_GET['reset'] == 1) {
    session::un_set('sogoManagement');
  }

  /* Show and save dialog */
  session::set('sogoManagement', $sogoManagement);
}
?>