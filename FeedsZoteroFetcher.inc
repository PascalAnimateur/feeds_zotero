<?php
/**
 * @file
 * FeedsZoteroFetcher.inc
 */

/**
 * Result of FeedsZoteroFetcher::fetch().
 */
class FeedsZoteroFetcherResult extends FeedsFetcherResult {

  /**
   * The fetched items.
   */
  protected $items;

  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct(NULL);
  }

  /**
   * Add items from JSON data.
   */
  public function addItems($json_data) {
    $item_types = _feeds_zotero_get_item_types();
    foreach (drupal_json_decode($json_data) as $item) {
      if (isset($item_types[$item['data']['itemType']])) {
        $this->items[$item['key']] = $item['data'];
      }
    }
  }

  public function getItems() {
    return $this->items;
  }
}

/**
 * Fetches Zotero data.
 */
class FeedsZoteroFetcher extends FeedsFetcher {

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
      '#options' => array('groups' => 'Group', 'users' => 'User'),
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

  /**
   * Fetch content from Zotero.
   */
  public function fetch(FeedsSource $source) {
    feeds_include_library('http_request.inc', 'http_request');
    $start = 0;
    $fetcher_result = new FeedsZoteroFetcherResult();

    while ($url = $this->getURL($start)) {
      $result = http_request_get($url);
      if (!in_array($result->code, array(200, 201, 202, 203, 204, 205, 206))) {
        throw new Exception(t('Download of @url failed with code !code. Error details: !details',
          array('@url' => $url, '!code' => $result->code, '!details' => $result->data)));
      }
      $fetcher_result->addItems($result->data);
      if (preg_match('/start=(\d+)>; rel="next"/', $result->headers['link'], $matches)) {
        $start = $matches[1];
      }
      else {
        break;
      }
    }

    return $fetcher_result;
  }

  /**
   * Get URL to retrieve items from Zotero API.
   */
  public function getURL($start = 0) {
    $zotero_type = $this->config['zotero_type'];
    $zotero_id = $this->config['zotero_id'];
    if (empty($zotero_type) || empty($zotero_id)) {
      return FALSE;
    }
    $url = "https://api.zotero.org/$zotero_type/$zotero_id/items?start=$start";
    if (!empty($this->config['api_key'])) {
      $url .= '&key=' . $this->config['api_key'];
    }

    return $url;
  }

}
