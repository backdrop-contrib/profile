Profile
========

Supports configurable user profiles.

This module is the successor to the [Drupal 7 Profile2 module](https://drupal.org/project/profile2).

Installation
-------------

* Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

Usage
-----

* Visit the configuration page under Administration > Structure >
  Profiles (admin/structure/profiles) for managing profile types.

* By default users may view their profile at /user and edit them at
   user/N/edit.

Upgrading from Drupal 7
-----------------------

This module is the successor to the [Drupal 7 Profile2
module](https://drupal.org/project/profile2) and will automatically import and
convert any Profile2 profiles when upgrading from a Drupal 7 installation.

Note that this module does NOT import from the [deprecated Profile
module](https://www.drupal.org/node/874026) that was provided in Drupal 7 core
as an upgrade path from Drupal 6 sites that used Profiles. If you have profiles
from the Drupal 7 Profile module, you should either convert them to Profile2
profiles in Drupal 7 prior to upgrading to Backdrop, or recreate them in
Backdrop after you upgrade.

Documentation
-------------

Additional documentation is located in the [Wiki](https://github.com/backdrop-contrib/profile/wiki/Documentation).

Issues
------

Bugs and feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/profile/issues).

License
---------------

This project is GPL v2 software. See the LICENSE.txt file in this directory
for complete text.


Current Maintainers
-------------------

* [Robert J. Lang](https://github.com/bugfolder)

Credits:
----------

* Ported to Backdrop CMS by [docwilmot](https://github.com/docwilmot).

* Original Drupal maintained module by
    * [Wolfgang Ziegler (fago)](https://www.drupal.org/u/fago), nuppla@zites.net
    * [Joachim Noreiko (joachim)](https://www.drupal.org/u/joachim), joachim.n+backdrop@gmail.com

