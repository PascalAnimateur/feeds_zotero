<?php
/**
 * @file
 * FeedsZoteroParser.inc
 */

class FeedsZoteroParser extends FeedsParser {
  /**
   * Parse JSON content fetched by FeedsZoteroFetcher.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $result = new FeedsParserResult();

    // Parse JSON items from $fetcher_result
    foreach ($fetcher_result->getItems() as $key => $item) {
      $result->items[$key] = $item;
    }

    return $result;
  }

  /**
   * Return mapping sources from available Zotero fields.
   */
  public function getMappingSources() {
    $sources = array();

    // Zotero item fields.
    foreach (_feeds_zotero_get_item_fields() as $field => $localized) {
      $sources[$field] = array('name' => $localized);
    }

    // Zotero creator types.
    foreach (_feeds_zotero_get_creator_types() as $creator_type => $localized) {
      $sources['creator:' . $creator_type] = array('name' => t('Creator (@type)', array('@type' => $localized)));
    }

    $sources['key'] = array(
      'name' => 'Zotero ID',
    );

    $sources['itemType'] = array(
      'name' => 'Item type',
    );

    return $sources;
  }

  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {
    $item = $result->currentItem();

    // Process creators.
    $keys = explode(':', $element_key);
    if ($keys[0] == 'creator') {
      $creators = array();
      foreach ($item['creators'] as $creator) {
        if ($creator['creatorType'] == $keys[1]) {
          if (isset($creator['name'])) {
            $creators[] = $creator['name'];
          }
          else {
            $creators[] = $creator['lastName'] . ', ' . $creator['firstName'];
          }
        }
      }
      return $creators;
    }

    return isset($item[$element_key]) ? $item[$element_key] : '';
  }

}
