<?php
/**
 * @file
 * FeedsZoteroHTTPFetcher.inc
 */

/**
 * Result of FeedsZoteroHTTPFetcher::fetch().
 */
class FeedsZoteroHTTPFetcherResult extends FeedsHTTPFetcherResult {
}

/**
 * Fetches Zotero data via HTTP.
 */
class FeedsZoteroHTTPFetcher extends FeedsHTTPFetcher {

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'zotero_type' => NULL,
      'zotero_id' => '',
      'api_key' => '',
    );
  }

  /**
   * Implements FeedsConfigurable::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();

    // Zotero Group / User library.
    $form['zotero_type'] = array(
      '#type' => 'radios',
      '#title' => t('What type of Zotero library is this?'),
      '#options' => array('group' => 'Group', 'user' => 'User'),
      '#default_value' => isset($this->config['zotero_type']) ? $this->config['zotero_type'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );
    $form['zotero_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Zotero ID'),
      '#description' => t('Enter the numeric id of your zotero user or group. To find your
            group id, navigate to your groups\'s settings page and the numeric part of the URL
            is you group id. Your userID is in
            <a href="https://www.zotero.org/settings/keys">Feeds/API section</a> of your zotero settings.'),
      '#default_value' => isset($this->config['zotero_id']) ? $this->config['zotero_id'] : '',
      '#maxlength' => NULL,
      '#required' => TRUE,
    );

    // Zotero API key.
    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Zotero API key'),
      '#description' => t('If your group or user account is private, you will need to enter
            your api key to be allowed to access the library.  Your api key is in
            <a href="https://www.zotero.org/settings/keys">Feeds/API section</a> of your zotero settings.'),
      '#default_value' => isset($this->config['api_key']) ? $this->config['api_key'] : '',
      '#maxlength' => NULL,
    );

    return $form;
  }
}
