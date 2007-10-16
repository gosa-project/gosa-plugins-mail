<?php 
class ArpNewDevice extends plugin
{
  var $cn= "";
  var $description = "";
  var $netConfigDNS = NULL;
  var $attributes= array("cn","description");
  var $objectclasses = array("gotoHard");

  var $ignore_account = TRUE;

  function ArpNewDevice ($config, $dn= NULL, $parent= NULL)
  {
    plugin :: plugin($config,$dn);
    $this->netConfigDNS = new termDNS($this->config,$this->dn,$this->objectclasses);
    $this->acl = "#all#";
    $this->netConfigDNS->acl = $this->acl;
  }

  function execute()
  {
    /* Display sub dialog from network settings */
    $this->netConfigDNS->acl = $this->acl;
    if($this->netConfigDNS->dialog){
      $this->dialog = TRUE;
      return($this->netConfigDNS->execute());
    }else{
      $this->dialog = FALSE;
    }

    $smarty = get_smarty();
    foreach($this->attributes as $attr){
      $smarty->assign($attr,$this->$attr);
    }
    $this->netConfigDNS->cn= $this->cn;
    $smarty->assign("netconfig", $this->netConfigDNS->execute());
    return($smarty->fetch (get_template_path('ArpNewDevice.tpl', TRUE)));
  }

  function check()
  {
    $message= plugin::check();
    $message= array_merge($message, $this->netConfigDNS->check());
    return($message);
  }

  function save_object()
  {
    if(isset($_POST['ArpNewDevice_posted'])){
      plugin::save_object();
      $this->netConfigDNS->save_object();
    }
  }

  function save()
  {
    $this->netConfigDNS->acl = $this->acl;
    plugin::save();
    $this->netConfigDNS->cn = $this->cn;    
    $this->netConfigDNS->save($this->dn);
    $ldap = $this->config->get_ldap_link();
    $ldap->cd($this->config->current['BASE']);
    $ldap->rmdir_recursive($this->dn);
    show_ldap_error($ldap->get_error(),_("Could not remove arp-alert device after writing dns and dhcp configuration."));
  }
}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>