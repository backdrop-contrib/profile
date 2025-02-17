<?php
/**
 * @file
 * Profile type editing UI.
 */

/**
 * Generate profile
 */
function profile_generate_form($form, &$form_state) {
  $types = profile_get_types();
  foreach ($types as $id => $type) {
    $types[$id] = $type->label;
  }

  $form['profile_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Profile types'),
    '#description' => t('Select profile types to create profile. If no types are selected, profile will be generated for all types.'),
    '#options' => $types,
  );

  $roles['[any]'] = t('any');
  $roles += user_roles(TRUE);
  unset($roles[BACKDROP_AUTHENTICATED_ROLE]); // Don't list authorized role.

  $form['profile_roles_select'] = array(
    '#type' => 'select',
    '#title' => t('The roles of user'),
    '#default_value' => -1,
    '#options' => $roles,
  );

  $form['profile_generate_limit_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of profiles per type.'),
    '#default_value' => 50,
    '#size' => 10,
  );

  $form['profile_kill'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace existing profiles.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );
  return $form;
}

/**
 * FormAPI submission to generate nodes.
 */
function profile_generate_form_submit($form, &$form_state) {
  $user_ids = array();
  if ($form_state['values']['profile_roles_select'] == '[any]') {
    // generate profile for all users.
    $users = user_load_multiple(FALSE);
    //don't generate profile for anonymous user (uid = 0).
    unset($users[0]);
    $user_ids = array_keys($users);
  }
  else {
    $role = $form_state['values']['profile_roles_select'];
    $query = db_select('users', 'u')->fields('u', array('uid'));
    $query->condition('u.uid', 0, '<>');
    $users = $query->execute()->fetchCol();
    $users = user_load_multiple($users);
    foreach ($users as $user) {
      if (in_array($role, $user->roles)) {
        $user_ids[$user->uid] = $user->uid;
      }
    }
  }

  $types = $form_state['values']['profile_types'];
  $kill = $form_state['values']['profile_kill'];
  $limit_number = $form_state['values']['profile_generate_limit_number'];

  if (count($user_ids) > 0) {
    $types = array_keys(array_filter($types));
    $pids = profile_generate_profile($user_ids, $types, $kill, $limit_number);
    backdrop_set_message(t("Generated %count profile(s).", array('%count' => count($pids))));
  }
}

/**
 * The main API function for creating profile.
 */
function profile_generate_profile($uids, $types, $kill, $limit_number) {
  if ($kill) {
    // get all profile and delete this.
    $profiles = profile_load_multiple(FALSE);
    $pids = array_keys($profiles);
    if ($pids) {
      // delete all profile first.
      profile_delete_multiple($pids);
    }
  }

  $new_pids = array();
  while (count($new_pids) < $limit_number && $type = array_shift($types)) {
    while(count($new_pids) < $limit_number && $uid = array_shift($uids)) {
      if (profile_load_by_user($uid, $type)) {
        continue;
      }
      $profile = entity_create('profile', array('type' => $type, 'uid' => $uid));
      // Populate all core fields on behalf of field.module
      module_load_include('inc', 'devel_generate', 'devel_generate.fields');
      module_load_include('inc', 'devel_generate', 'devel_generate');
      // add language for profile object.
      // @see devel_generate_fields
      $profile->language = LANGUAGE_NONE;
      devel_generate_fields($profile, 'profile', $profile->type);
      profile_save($profile);
      $new_pids[] = $profile->pid;
    }
  }

  return $new_pids;
}
