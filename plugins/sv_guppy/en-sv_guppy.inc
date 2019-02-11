<?php
/*
    Sauvez Guppy plugin pour Guppy par Stéphane GOSNET (http://stephane.gosnet.free.fr)
	(inspiré du plugin Save de Nicolas Alves http://www.con-est-con.com )
	
	Sauvegarde de site Guppy

	Copyright (c) 2016 <Stéphane GOSNET> (http://stephane.gosnet.free.fr)

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
	
    Historique de Versions :
      Pré-version v0.10 Octobre 2009 (GuppY 4.6.10)
      Version v1.00 Novembre 2009 (GuppY 4.6.10)
	  Version v1.1.0 Septembre 2012 (Guppy 4.6.10)
	  Version v1.2.0 Juin 2016 (Guppy 5.0)

    Ce fichier : langue anglaise
*/

// security
if (stristr($_SERVER["SCRIPT_NAME"], "en-sv_guppy.inc")) {
  header("location:../index.php");
  die();
}

$sv_guppy[0] =  "Admin - Sauvez Guppy";
$sv_guppy[1] =  "Sauvez Guppy options";
$sv_guppy[2] =  "Sauvez Guppy";
$sv_guppy[3] =  "Activate";
$sv_guppy[4] =  "Password";
$sv_guppy[5] =  "Keep";
$sv_guppy[6] =  "Folder to save";
$sv_guppy[7] =  "File(s)";
$sv_guppy[8] =  "Sauvez Guppy Explorer";
$sv_guppy[9] =  "To plan an offline backup, use this links :";
$sv_guppy[10] =  "Go !";
$sv_guppy[11] =  "Explorer - Sauvez Guppy";
$sv_guppy[12] =  "Archives Files";
$sv_guppy[13] =  "Delete archives";
$sv_guppy[14] =  "About - Sauvez Guppy";
$sv_guppy[15] =  "With download :";
$sv_guppy[16] =  "Witout download :";
$sv_guppy[17] =  "Replace [*] with selection number.";
$sv_guppy[18] =  "Save config";
$sv_guppy[19] =  "No backup";
$sv_guppy[20] =  "Uninstall Sauvez Guppy !!";
?>
