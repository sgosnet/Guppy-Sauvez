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

    Ce fichier : Admin du plugin

*/

# Sécurité
if (stristr($_SERVER["SCRIPT_NAME"], "sv_guppy_admin.inc")) {
  header("location:../index.php");
  die();
}

# Désinstallation
$uninstall = import('uninstall');
if ($uninstall == 'on') {
	include CHEMIN.'admin/plugins/sv_guppy/uninstall.inc';
	ap_uninstall();
}

# Fichier de langue et fonctions
if (is_file(CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc')) {
    include CHEMIN.'plugins/sv_guppy/'.$lng.'-sv_guppy.inc';
} else {
    include CHEMIN.'plugins/sv_guppy/en-sv_guppy.inc';
}
include("plugins/funcplug.inc");
include("sv_guppy_func.inc");
include("version/update.inc");

# Sauvegardes de la configuration
$FIC_OPTIONS=CHEMIN.'plugins/sv_guppy/sv_guppy_options.inc';
$FIC_CLASSES=CHEMIN.'plugins/sv_guppy/sv_guppy_classes.inc';
$NBRE_CLASSES=5;

if($regit==1){
	# Activation du plugin
	$svpEtat="";
	$svpEtat=import('svpEtat');
	if ($svpEtat == 'on') {
		if (!PluginRegistered('sv_guppy'))
			RegisterPlugin('sv_guppy','ALL','ALL');
	}
		else
			UnregisterPlugin('sv_guppy');
	# Sauvegarde des options et classes
	$svpOptions['sesam']=import('svpSesam');
  	$svpOptions['historique']=import('svpHistorique');
    	$svpNbClasses = import('nb_classes');
  	savePlusSetOptions($FIC_OPTIONS,$svpOptions);

	# Sauvegarde des sélections de dossiers
	$repertoires = savePlusGetRepertoires(CHEMIN);
	for($nClasse=0;$nClasse<$NBRE_CLASSES;$nClasse++){
		for($cnt=0;$cnt<$svpNbClasses;$cnt++){
			if(import('classe'.$nClasse.'_'.$cnt)=="on"){
				$classes[$nClasse][$cnt]=$repertoires[$cnt];
			}
		}
	}
	savePlusSetClasses($FIC_CLASSES,$classes);
}

# Lecture des options et classes
$svpOptions=savePlusGetOptions($FIC_OPTIONS);
$repertoires = savePlusGetRepertoires(CHEMIN);
$classes = savePlusGetClasses($FIC_CLASSES);

# Affichage

# Top
$topmess = '<a href="admin.php?lng='.$lng.'">Admin</a> > '.str_replace('Admin - ', '', $sv_guppy[0]);
include CHEMIN.'admin/hpage.inc';
if ($wri == 'admin') include CHEMIN.'admin/administrateur.php';
if ($wri == $userprefs[1]) include CHEMIN.'admin/adminredac.php';
htable($sv_guppy[0], '100%');

# Formulaire des options
echo '
<p align="center"><b>'.$sv_guppy[1].'</b></p>
<form name="sv_guppy" action="admin.php?lng='.$lng.'&amp;pg=plugin&amp;plug=sv_guppy/sv_guppy_admin" method="post">
<input type="hidden" name="regit" value="1" />
<table cellspacing="1" cellpadding="5" border="0" align="center">
	<tr><td>
	<table cellspacing="1" cellpadding="1" border="0" align="center">
		<tr>
			<td><p>'.$sv_guppy[3].'</p></td>
			<td><input type="checkbox" name="svpEtat" '.Checked(PluginRegistered("sv_guppy")).' /></td>
		</tr>
	  	<tr>
			<td><p>'.$sv_guppy[4].'</p></td>
			<td><input type="input" name="svpSesam" value="'.htmlentities($svpOptions['sesam']).'" /></td>
		</tr>
	  	<tr>
			<td><p>'.$sv_guppy[5].'</p></td>
			<td><select name="svpHistorique">';
	  			for($i=1;$i<20;$i++)
				{
					echo "<option value='$i'";
					if($svpOptions['historique']==$i)
						echo " selected ";
					echo ">$i</option>\n";
				}
echo '
			</select>'.$sv_guppy[7].'</td>
	</table>
	</td></tr>
</table>
';

# Formulaire dossiers
echo '
<p align="center"><b>'.$sv_guppy[6].'</b></p>
<table cellspacing="1" cellpadding="2" border="0" align="center">
    <tr><td>
        <table width="60%" cellspacing="0" cellpadding="2" border="0" align="center">
            <tr>
                <td></td>';
	for($nClasse=0;$nClasse<$NBRE_CLASSES;$nClasse++){
		echo "<td>".$nClasse."</td>";
	}
    echo '</tr>';
	$cnt=0;
	foreach($repertoires as $repertoire){
		echo '<tr><td>'.$repertoire;
		for($nClasse=0;$nClasse<$NBRE_CLASSES;$nClasse++){
			echo "</td><td><input name='classe".$nClasse."_".$cnt."' type='checkbox'";
			if ($repertoire == "save+") echo ' disabled ';
			if ($classes[$nClasse][$repertoire]==1) echo ' checked />'; else echo "/>";
		}
		echo "</td></tr>";
		$cnt++;
}


# Liens de lancement des sauvegardes
if(PluginRegistered('sv_guppy')){
echo '
		<tr>
            <td><b>'.$sv_guppy[10].'</b></td>
';
for($nClasse=0;$nClasse<$NBRE_CLASSES;$nClasse++){
    echo "<td><a href=".CHEMIN."plugins/sv_guppy/sv_guppy.php?selection=".$nClasse."&sesam=".urlencode($svpOptions['sesam'])."><img border ='0' src='".CHEMIN."/admin/plugins/sv_guppy/go.png'></a></td>";
}
echo '
        </tr>
';
}

# Bouton enregistrer
echo '
    </table>
    <input type="hidden" name="nb_classes" value="'.$cnt.'" />
    <p align="center">'.$boutonleft.'<input class="bouton" type="submit" value="'.$sv_guppy[18].'" />'.$boutonright.'</p>
</table>
</form>
';

# Lien vers autre s fonctions Sauvez Guppy
echo '
<p align="center"><a href="admin.php?lng='. $lng.'&pg=plugin&plug=sv_guppy/sv_guppy_explorer">'.$sv_guppy[11].'</a></p>
';

# Info pour plannifier une sauvegarde
if(PluginRegistered('sv_guppy')){
	echo '
	<hr /><p align="center">'.$sv_guppy[9].'</br></p>
	<p></br><u>'.$sv_guppy[15].'</u></br><b>'.$site[3].'plugins/sv_guppy/sv_guppy.php?selection=<i>[*]</i>&sesam='.urlencode($svpOptions['sesam']).'&offline=1</b></br></p>
	<p></br><u>'.$sv_guppy[16].'</u></br><b>'.$site[3].'plugins/sv_guppy/sv_guppy.php?selection=<i>[*]</i>&sesam='.urlencode($svpOptions['sesam']).'&offline=1&download=1</b></br></p>
	<p></br><i>'.$sv_guppy[17].'</i></p>
	</p><br />';
}


# Info version
echo '
<hr />
<div style="margin:20px;text-align:center;">
	<p>Sauvez Guppy &copy; Stephane GOSNET</p>
	<p>v'.$version['current'].' - '.$version['date'].'</p>
	<p>'.$version['errors'].$version['ok'].'</p>
</div>
';

# Coprs désinstallation
echo '
<div style="margin:20px;text-align:center;">
  <form name="uninst" action="admin.php?lng='.$lng.'&amp;pg=plugin&amp;plug=sv_guppy/sv_guppy_admin" method="POST">
	<input type="hidden" name="uninstall" value="on" />
	'.SubmitButton($sv_guppy[20]).'
  </form>
</div>
<hr />';


# Bas de page
echo '
<p style="text-align:center"><a href="admin.php?lng='.$lng.'">'.$admin[22].'</a> || <b><a href="admin.php?lng='.$lng.'&amp;logout=1">'.$admin[19].'</a></b></p>';

btable();
include CHEMIN.'admin/bpage.inc';

