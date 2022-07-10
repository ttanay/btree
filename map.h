#include <initializer_list>
#include <stdexcept>
#include <string>
#include "btree.h"

namespace btree
{
template <typename K, typename V>
struct MapElement
{
  K key;
  V value;

  bool operator>(const MapElement & other) { return key > other.key; }

  bool operator<(const MapElement & other) { return key < other.key; }

  MapElement & operator=(V & new_value) { value = new_value; }

  bool operator==(const MapElement & other) { return key == other.key; }
};

template <typename K, typename V>
class Map
{
  using MapBTree = BTree<MapElement<K, V>>;
  BTree<MapElement<K, V>> btree;

public:
  Map(std::initializer_list<MapElement<K, V>> l)
  {
    for (auto element : l)
      btree.insert(element);
  }

  // Element Access
  const V at(const K & key);
  MapElement<K, V> operator[](const K & key);

  // Modifiers
  void clear() { btree.clear(); }
  void insert(K key, V value);
  void erase(K key);
};

template <typename K, typename V>
const V Map<K, V>::at(const K & key)
{
  auto dummy = MapElement<K, V>{key, V()};
  auto result = btree.search(dummy);
  if (!result.isEmpty())
  {
    auto element = result.node->keys[result.index];
    return element.value;
  }
  else
    throw std::out_of_range("key doesn't exist");
}

template <typename K, typename V>
MapElement<K, V> Map<K, V>::operator[](const K & key)
{
  auto search_key = MapElement<K, V>{key};
  auto result = btree.search(search_key);
  if (result.isEmpty())
  {
    btree.insert(search_key);
    return search_key;
  }
  else
    return result.node->keys[result.index];
}

template <typename K, typename V>
void Map<K, V>::insert(K key, V value)
{
  btree.insert(MapElement<K, V>{key, value});
}

template <typename K, typename V>
void Map<K, V>::erase(K key)
{
  btree.del(MapElement<K, V>{key});
}

}
