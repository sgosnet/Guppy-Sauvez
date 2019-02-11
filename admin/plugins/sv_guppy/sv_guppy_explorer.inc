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

    Ce fichier : explorateur des archives
*/

# Sécurité
if (stristr($_SERVER["SCRIPT_NAME"], "sv_guppy_admin.inc")) {
  header("location:../index.php");
  die();
}

# Fichier de langue et fonctions
if (is_file(CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc')) {
    include CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc';
} else {
    include CHEMIN.'plugins/sv_guppy/en-sv_guppy.inc';
}
include("plugins/funcplug.inc");
include("sv_guppy_func.inc");
$ARCHIVES = CHEMIN."save+";

# Vérification de la présence du dossier Save+
if(!is_dir($ARCHIVES))
	mkdir($ARCHIVES);

#Suppressison des archives sélectionnées
$svpSupprime = import('supprime');
$svpNbArchives = import('nb_archives');
if($svpSupprime=="1"){
	for($cnt=0;$cnt<$svpNbArchives;$cnt++){
		if(import("supprime$cnt")=="on")
			unlink($ARCHIVES."/".import("archive$cnt"));
	}
}

# Lecture des fichiers d'archives
$archives = savePlusGetArchives($ARCHIVES);

# Affichage

# Top
$topmess = '<a href="admin.php?lng='.$lng.'">Admin</a> > '.str_replace('Admin - ', '', $sv_guppy[0]);
include CHEMIN.'admin/hpage.inc';
if ($wri == 'admin') include CHEMIN.'admin/administrateur.php';
if ($wri == $userprefs[1]) include CHEMIN.'admin/adminredac.php';
htable($sv_guppy[0], '100%');

echo '
<p style="text-align:center"><b>'.$sv_guppy[12].'</b></p>

';

if($archives){
	$cnt=0;
	$nb=0;

	echo '
<form name="sv_guppy" action="admin.php?lng='.$lng.'&amp;pg=plugin&amp;plug=sv_guppy/sv_guppy_explorer" method="post">
	<input type="hidden" name="supprime" value="1" />
	<table width="60%" cellspacing="0" cellpadding="2" border="0" align="center">';

# Parcours des archives avec While pour récupérer le numéro d'index du tableau par key(). Ne fonctionne pas avec Forearch()
while((!is_null($dossier = key($archives)))){
	if(!is_array($archives[$dossier])){
		$nb++;
		$archive = $archives[$dossier];
		echo "<input type='hidden' name='archive".$cnt."' value='".$archive."' />";
		echo "<tr><td><p><b>$nb&nbsp;</b><a href='".CHEMIN."save+/".$archive."'><img src='".CHEMIN."/admin/plugins/sv_guppy/archive.gif'/>&nbsp;".$archive."</a></td><td><input name='supprime".$cnt."' type='checkbox'/>";
		echo "</p></td></tr>";
		$cnt++;
	}else{
		$nb=0;
		$archive = $archives[$dossier];
		echo "<tr><td colspan='2' align='center'><b>$world6 $dossier</b></td></tr>";
		foreach($archive as $arch){
			$nb++;
			echo "<input type='hidden' name='archive".$cnt."' value='".$dossier."/".$arch."' />";
			echo "<tr><td><p><b>$nb&nbsp;</b><a href='".CHEMIN."save+/".$dossier."/".$arch."'><img border='0' src='".CHEMIN."/admin/plugins/sv_guppy/archive.png'/>&nbsp;".$arch."</a></td><td><input name='supprime".$cnt."' type='checkbox'/>";
			echo "</p></td></tr>";
			$cnt++;			
		}
	}
	next($archives);
}

echo '
	</table>
	<input type="hidden" name="nb_archives" value="'.$cnt.'" />
	<p align="center">'.$boutonleft.'<input class="bouton" type="submit" value="'.$sv_guppy[13].'" />'.$boutonright.'</p>
</form>
';
}else{
	echo '<p align="center">'.$sv_guppy[19].'</p>';
}

# Lien vers autre s fonctions Sauvez Guppy
echo '
<p align="center"><a href="admin.php?lng='. $lng.'&pg=plugin&plug=sv_guppy/sv_guppy_admin">'.$sv_guppy[0].'</a></p>
';

# Bas de page
echo '
<p style="text-align:center"><a href="admin.php?lng='.$lng.'">'.$admin[22].'</a> || <b><a href="admin.php?lng='.$lng.'&amp;logout=1">'.$admin[19].'</a></b></p>';

btable();
include CHEMIN.'admin/bpage.inc';
