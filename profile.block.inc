<?php
/**
 *  ProfileBlock extends Block
 *
 *  This class allows us to create Profile blocks.
 */
class ProfileBlock extends Block {

  /**
   *  Sets title text on draggable block panel in Layout builder.
   */
  function getAdminTitle() {
    if (!empty($this->settings['admin_label'])) {
      return check_plain($this->settings['admin_label']);
    }

    $children = $this->getChildren();
    $title = $children[$this->childDelta]['info'];
    return strlen($this->settings['title']) ? check_plain($this->settings['title']) : $title;
  }

  /**
   *  Sets block subject on block view.
   */
  function getTitle() {
    return isset($this->settings['title']) ? check_plain($this->settings['title']) : '';
  }

  /**
   * Returns the rendered content of this block.
   *
   * @return array | null
   */
  function getContent() {
    $type = $this->childDelta;
    $profile_type = profile_get_types($type);
    $account = $this->contexts['user']->data;
    if ($profile_type->userview && $profile = profile_load_by_user($account, $type)) {
      if (profile_access('view', $profile)) {
        $build['profile_' . $type] = array(
          '#type' => 'item',
          '#title' => $profile_type->label,
          '#prefix' => '<a class="profile-' . $profile->type . '"></a>',
        );
        $render = $profile->view();
        $build['profile_' . $type]['#markup'] = render($render);
        return $build;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  function getChildren() {
    $blocks = [];
    foreach (profile_get_types() as $type => $profile_type) {
      $blocks[$type] = array(
        'info' => t('Profile !label block', array('!label' => $profile_type->label)),
        'description' => t('Provides Profile data as blocks.'),
      );
    }
    return $blocks;
  }
}
