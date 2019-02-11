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

   Ce fichier : desinstallation du plugin

*/


if (stristr($_SERVER['SCRIPT_NAME'], 'uninstall.inc')) {
	header('location:../index.php');
	die();
}


function ap_uninstall() {
	include CHEMIN.'admin/plugins/sv_guppy/plugin.inc';
	foreach ($plugin_datas_uninstall['dirs'] as $dir)
		RecursiveRemoteDir(CHEMIN.$dir);
	if (!empty($plugin_datas_uninstall['unreg'])) {
		$regs = ReadDBFields(DBPLUGIN);
		$tmp  = array();
		foreach ($regs as $reg)
			if ($reg[0] != $plugin_datas_uninstall['unreg'])
				$tmp[] = $reg;
		WriteDBFields(DBPLUGIN, $tmp);
	}

	header('location:'.CHEMIN.'admin/admin.php?lng='.$lng);
	die();
}
?>
