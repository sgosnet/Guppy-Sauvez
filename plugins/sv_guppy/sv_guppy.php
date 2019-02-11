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

	Ce fichier : Script de sauvegarde
*/


header('Pragma: no-cache');
define('CHEMIN', '../../');
include CHEMIN.'inc/includes.inc';

if (is_file(CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc')) {
    include CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc';
} else {
    include CHEMIN.'plugins/sv_guppy/en-sv_guppy.inc';
}

include(CHEMIN."admin/plugins/sv_guppy/sv_guppy_func.inc");
include(CHEMIN."admin/plugins/funcplug.inc");
include ("pclzip.lib.php");

$svgComplet=False;

# Lecture des options de Save+
$FIC_CLASSES="sv_guppy_classes.inc";
$FIC_OPTIONS="sv_guppy_options.inc";
$options=savePlusGetOptions($FIC_OPTIONS);

# Lecteur des paramètres de sauvegardes
$svgSesam=import('sesam');
$svgSelection = import('selection');
$svgOffline=import('offline');
$svgDownload=import('download');

# Controle Activation plugin et Sesam
if (!(($svgSesam==$options['sesam']) && PluginRegistered('sv_guppy'))){
	header("location:../index.php");
	die();
}

$fichier =date("YmdHis-").md5($options['sesam']).".zip";
$archive_name = CHEMIN."save+/".$svgSelection."/".$fichier;
	// Vérification de la présence du dossier Save+
	if(!is_dir(CHEMIN."save+"))
		mkdir(CHEMIN."save+");
	if(!is_file(CHEMIN."save+/index.php")){
		$handle=fopen(CHEMIN."save+/index.php",'w');
		fputs($handle,'<?php header(\'location:../index.php\');?>');
	}
	if(!is_file(CHEMIN."save+/index.php")){
		die("Error : 010 : Creating Save+ folder");
	}

	# Préparation sauvegarde selon N° de sélection demandée
	if(!is_dir(CHEMIN."save+/".$svgSelection))
		mkdir(CHEMIN."save+/".$svgSelection);
	if(!is_file(CHEMIN."save+/".$svgSelection."/index.php")){
		$handle=fopen(CHEMIN."save+/".$svgSelection."/index.php",'w');
		fputs($handle,'<?php header(\'location:../index.php\');?>');
	}
	if(!is_file(CHEMIN."save+/".$svgSelection."/index.php")){
		die("Error : 010 : Creating Save+ folder");
	}

	# Suppression des échecs sauvegardes précédentes
	if(False==savePlusSupprTempFiles('.')) die("Error 030 : Deleting Temp files.");

	# Suppression de l'historique
	if(-1==savePlusSupprHistorique(CHEMIN."save+/".$svgSelection,$options['historique']-1)) die("Error 020 : Deleting History Archives"); // Historique -1 => restant + sauvegarde à faire.
	
	# Détermination des fichiers et dossiers à sauvegarder
	$repertoires = savePlusGetRepertoires(CHEMIN);
	$classes = savePlusGetClasses($FIC_CLASSES);
	$sessions = saveGetSessions(CHEMIN,$classes,$svgSelection);
	
	# Sauvegarde
	if($sessions != False){
		$zip = new PclZip($archive_name);
		$debut=True;
		foreach($sessions as $session){
			if($debut){
				$zip->create($session);
				$debut=False;
			}else{
				$zip->add($session);
			}
		}
		$svgComplet=True; // Sauvegarde terminée
	}

	# Sauvegarde Offline : envoi Zip ou vers Explorateur des sauvegardes
	if($svgOffline){
	    if($svgDownload){
			header("location:$archive_name");
			die();
		}else{
			die($svgComplet);
		}
	}else
		header("location:".CHEMIN."admin/admin.php?lng=fr&pg=plugin&plug=sv_guppy/sv_guppy_explorer");


?>
