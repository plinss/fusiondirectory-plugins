<?php
/*
 * This code is part of FusionDirectory (http://www.fusiondirectory.org)
 * Copyright (C) 2014 FusionDirectory
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

class webapp extends simplePlugin
{
  var $mainTab = TRUE;

  var $objectclasses = array('fdWebApplication');

  /* Return plugin informations for acl handling  */
  public static function plInfo()
  {
    return array(
      'plShortName'   => _('Web application'),
      'plDescription' => _('Web applicationts names, icons and links'),
      'plObjectType'  => array('webapp' => array(
        'name'        => _('Web application'),
        'filter'      => 'objectClass=fdWebApplication',
        'ou'          => get_ou('webappsRDN'),
        'icon'        => 'geticon.php?context=categories&icon=applications-internet&size=16'
      )),

      'plProvidedAcls' => parent::generatePlProvidedAcls(self::getAttributesInfo())
    );
  }

  static function getAttributesInfo ()
  {
    return array(
      'main' => array(
        'name'  => _('Application'),
        'attrs' => array(
          new BaseSelectorAttribute (get_ou('webappsRDN')),
          new HostNameAttribute (
            _('Name'), _('Name or id for application'),
            'cn', TRUE
          ),
          new URLAttribute (
            _('URL'), _('Link to this application'),
            'labeledURI', TRUE
          ),
          new StringAttribute (
            _('Title'), _('Displayed name for links to this application'),
            'fdApplicationTitle', FALSE
          ),
          new ImageAttribute (
            _('Icon'), _('Icon for this application'),
            'fdApplicationImage', FALSE,
            48, 48, 'png', TRUE
          ),
          new PathAttribute (
            _('Icon location'), _('Usual path to this application icon'),
            'fdApplicationImageLocation', FALSE
          ),
        )
      ),
    );
  }

  function __construct (&$config, $dn = NULL, $object = NULL)
  {
    parent::__construct($config, $dn, $object);
  }

  /* Returns the dn of the entry */
  function compute_dn()
  {
    return 'cn='.$this->cn.','.get_ou('webappsRDN').$this->base;
  }
}
?>