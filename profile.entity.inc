<?php
/**
 * @file
 * Provides Entity integration.
 */

/**
 * The class used for profile entities.
 */
class Profile extends Entity {

  /**
   * The profile id.
   *
   * @var integer
   */
  public $pid;

  /**
   * The name of the profile type.
   *
   * @var string
   */
  public $type;

  /**
   * The profile label.
   *
   * @var string
   */
  public $label;

  /**
   * The user id of the profile owner.
   *
   * @var integer
   */
  public $uid;

  /**
   * The Unix timestamp when the profile was created.
   *
   * @var integer
   */
  public $created;

  /**
   * The Unix timestamp when the profile was most recently saved.
   *
   * @var integer
   */
  public $changed;

  /**
   * The language under which the profile item is stored.
   *
   * @var string
   */
  public $langcode = LANGUAGE_NONE;

  public function __construct(array $values = array()) {
    if (isset($values['user'])) {
      $this->setUser($values['user']);
      unset($values['user']);
    }
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->type;
    }
    if (!isset($values['label']) && isset($values['type']) && $type = profile_get_types($values['type'])) {
      // Initialize the label with the type label, so newly created profiles
      // have that as interim label.
      $values['label'] = $type->label;
    }
    parent::__construct($values);
  }

  /**
   * Returns the user owning this profile.
   */
  public function user() {
    return user_load($this->uid);
  }

  /**
   * Sets a new user owning this profile.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setUser($account) {
    $this->uid = is_object($account) ? $account->uid : $account;
  }

  /**
   * Gets the associated profile type object.
   *
   * @return ProfileType
   */
  public function type() {
    return profile_get_types($this->type);
  }

  /**
   * Returns the profile type for this profile.
   */
  public function bundle() {
    return $this->type;
  }

  /**
   * Returns the full url() for the profile.
   */
  public function url() {
    $uri = $this->uri();
    return url($uri['path'], $uri);
  }

  /**
   * Returns the backdrop path to this profile.
   */
  public function path() {
    $uri = $this->uri();
    return $uri['path'];
  }

  public function uri() {
    return array(
      'path' => 'user/' . $this->uid,
      'options' => array('fragment' => 'profile-' . $this->type),
    );
  }

  public function defaultLabel() {
    if (module_exists('profile_i18n')) {
      // Run the label through i18n_string() using the profile_type label
      // context, so the default label (= the type's label) gets translated.
      return entity_i18n_string('profile:profile_type:' . $this->type . ':label', $this->label);
    }
    return $this->label;
  }

  public function view($view_mode = 'full', $langcode = NULL, $page = NULL) {
    field_attach_prepare_view('profile', array($this->pid => $this), $view_mode);
    entity_prepare_view('profile', array($this));
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->langcode;

    $build = $this->buildContent($view_mode, $langcode);
    $build += array(
      '#theme' => 'profile',
      '#profile' => $this,
      '#entity' => $this,
      '#view_mode' => $view_mode,
      '#langcode' => $langcode,
    );

    $view = array();
    $key = isset($this->{$this->pid}) ? $this->{$this->pid} : NULL;
    $view['profile'][$key] = $build;

    return $view;
  }

  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();
    // Assume newly create objects are still empty.
    if (!empty($this->is_new)) {
      $content['empty']['#markup'] = '<em class="profile2-no-data">' . t('There is no profile data yet.') . '</em>';
    }
    // Remove previously built content, if exists.
    $this->content = $content;
    $langcode = isset($langcode) ? $langcode : $GLOBALS['language_content']->langcode;

    // Make sure the used view-mode gets stored.
    $this->content += array('#view_mode' => $view_mode);

    // Add in fields.
    $key = isset($this->{$this->pid}) ? $this->{$this->pid} : NULL;
    field_attach_prepare_view('profile', array($key => $this), $view_mode);
    $this->content += field_attach_view('profile', $this, $view_mode, $langcode);

    module_invoke_all('profile' . '_view', $this, $view_mode, $langcode);

    module_invoke_all('entity_view', $this, 'profile', $view_mode, $langcode);
    $build = $this->content;
    unset($this->content);

    return $build;
  }

  public function save() {
    // Care about setting created and changed values. But do not automatically
    // set a created values for already existing profiles.
    if (empty($this->created) && (!empty($this->is_new) || !$this->pid)) {
      $this->created = REQUEST_TIME;
    }
    $this->changed = REQUEST_TIME;

    $result = parent::save();
    // Update the static cache from profile_load_by_user().
    $cache = &backdrop_static('profile_load_by_user', array());
    if (isset($cache[$this->uid])) {
      $cache[$this->uid][$this->type] = $this->pid;
    }
    return $result;
  }
  /**
   * Implements EntityInterface::id().
   */
  public function id() {
    return $this->pid;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'profile';
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return $this->label;
  }

}
