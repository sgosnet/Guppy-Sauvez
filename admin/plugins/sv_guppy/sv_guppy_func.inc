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

    Ce fichier : fonctions de Sauvez Guppy
*/

// ********************************************
// Lecture des options de Sauvez Guppy
// En entrée : nom du fichier des options
// En sortie : tableau associatif des options
function savePlusGetOptions($fic_options){

	$DBFields=ReadDBFields($fic_options);
	$options['sesam']=urldecode($DBFields[0][0]);
	$options['historique']=$DBFields[0][1];
	return $options;
}

// ********************************************
// Enregistrement des options de Sauvez Guppy
// En entrée : nom du fichier des options, tableau associatif des options
// Retourne False en cas d'échec
function savePlusSetOptions($fic_options,$options){

	$DBFields[0][0]=urlencode($options['sesam']);
	$DBFields[0][1]=$options['historique'];
	return WriteDBFields($fic_options,$DBFields);
}


// ********************************************
// Lecture des répertoires du site
// En entrée : racine du site
// En sortie : tableau associatif répertoires présents à la racine
// 		False en cas d'échec
function savePlusGetRepertoires($racine_site){
	// Ouverture du répertoire racine
	$racine = opendir($racine_site); 
	if($racine==False) return False;
	// Initialisation répertoire racine
	$cnt = 1;
	$rep[0]='.';
	// Lecture du répertoire racine
	while($file = readdir($racine)) {
		if($file != "." && $file != ".." && is_dir($racine_site.$file)){
			$rep[$cnt]=$file;
			$cnt++;
		}
	}
	closedir($racine);
	// Tri des répertoires 
	if($cnt>0) sort($rep);
	return $rep;
}

// ********************************************
// Lecture des répertoires à sauvegarder par Sauvez Guppy
// En entrée : nom du fichier des classes
// En sortie : tableau associatif des répertoires à sauvegarder
function savePlusGetClasses($fic_classes){
	$DBFields=ReadDBFields($fic_classes);
	if(sizeof($DBFields)==0) return False;

	$nb=0;
	foreach($DBFields as $DBField){
		foreach($DBFields[$nb] as $classe)
		{
			$classes[$nb][$classe]=1;
		}
		$nb++;
	}
	return $classes;
}

// ********************************************
// Enregistrement des dossiers à sauvegarder par Sauvez Guppy
// En entrée : nom du fichier des classes, tableau des répertoires à sauvegarder
function savePlusSetClasses($fic_classes,$classes){
	if(sizeof($classes)){
		$nb=0;
		foreach($classes as $classe){
			$i=0;
			foreach($classe as $fichier){
				$DBFields[$nb][$i]=$fichier;
				$i++;
			}
			$nb++;
		}
	}
	WriteDBFields($fic_classes,$DBFields);
}

// Détermination des dossiers réels à sauvegarder
// En entrée : racine du site, nom du fichier des classes
// En sortie : tableau des fichiers et dossiers de premier niveau à sauvegarder
function saveGetSessions($racine_site,$classes,$selection){
	$cnt = 0;
	$sessions=False;
	
	$classes=$classes[$selection];
	// Ajout des fichiers de la racine
	if(isset($classes['.']) && $classes['.']==1){
		if(False==($racine = opendir($racine_site))) return False;;
		while($file = readdir($racine)) {
			if($file != "." && $file != ".." && !is_dir($racine_site.$file)){
				$sessions[$cnt]=$racine_site.$file;
				$cnt++;
			}
		}
		closedir($racine);
	}
	// Ajout des répertoires
	if(False == ($repertoires=savePlusGetRepertoires($racine_site))) return False;;
	foreach($repertoires as $repertoire){
		if($repertoire != "." && isset($classes[$repertoire]) && $classes[$repertoire]==1){
			$sessions[$cnt]=$racine_site.$repertoire;
			$cnt++;
		}			
	}
	return $sessions;
}

// *********************************************
// Lecture fichiers d'Archive de Sauvez Guppy
// En entrée : dossier des archives de Sauvez Guppy
// En sortie : tableau des noms des fichiers d'archive ou False si échec ou 0
function savePlusGetArchives($dossier_archives){
/*  Ne fonctionne pas chez Free car Glop() non supportée
	if(False==($fichiers = glob($dossier_archives."/*.zip"))) return False;
	$cnt=0;
	foreach($fichiers as $fichier){
	    $archives[$cnt]=substr($fichier,strlen($dossier_archives."/"));
	    $cnt++;
	}
	return $archives;
*/
	if(False==($dossier = opendir($dossier_archives))) return False;;
	$cnt=0;
	while(false !== ($file = readdir($dossier))) {
		if(!is_dir($dossier_archives."/".$file) && $file != 'index.php'){
			$archives[$cnt]=$file;
			$cnt++;
		}
		if((is_dir($dossier_archives."/".$file) && !(($file == '.') || ($file == '..')))){
			$contenu=savePlusGetArchives($dossier_archives."/".$file);
			if($contenu) $archives[$file]=$contenu;
		}
	}
	closedir($dossier);
	if (count($archives)){
		return $archives;
	}else
		return false;
}

// *********************************************
// Suppression des archives expirées
// En entrée : dossier des archives de Sauvez Guppy, nombre de rétention
// En sortie : Nombre de fichiers supprimés ou -1 si échec
function savePlusSupprHistorique($dossier_archives,$historique){
	// Lecture de archives
	if(False==($archives = savePlusGetArchives($dossier_archives))) return False;
	// Calcul nombre d'archives à supprimer
	$nb_a_effacer=0;
	$nb_a_effacer=count($archives)-$historique;
	// Boucle de suppression
	for($i=0;$i<$nb_a_effacer;$i++){ // Si $nb_a_effacer < 0, on ne rentre pas dans la boucle
		if(is_file($dossier_archives."/".$archives[$i])){
			if(False==(unlink($dossier_archives."/".$archives[$i]))) return -1;
		}
	}
	return $i;
}

// *********************************************
// Suppression des fichiers PclZip temporaire
// En entrée : dossier des archives de Sauvez Guppy, nombre de rétention
// En sortie : Nombre de fichiers supprimés ou -1 si échec
function savePlusSupprTempFiles($dossier){
/* Ne fonctionne pas chez Free car Glop() non supportée

	if(False==($fichiers=glob("pclzip-*.tmp"))) return False;
	foreach($fichiers as $fichier){
		if(False==unlink($fichier))return False;
	}
	return True;
*/
	if(False==($dossier = opendir($dossier))) return False;;
	$cnt=0;
	while(False!=($file = readdir($dossier))){
		if((substr($file,0,7) == 'pclzip-') && (substr($file,strlen($file)-4,4)=='.tmp')){
			if(False==unlink($file))return False;
		}
	}
	closedir($dossier);
	return True;
}

?>
