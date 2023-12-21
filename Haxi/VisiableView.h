#pragma once
#include <vector>
#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QString>
#include <QWheelEvent>
#include <QRandomGenerator>

template <typename K, typename V>
class HashNode {
public:
	K key;
	V value;
	HashNode* next;

	HashNode(const K& key, const V& value) : key(key), value(value), next(nullptr) {}
};


template <typename K, typename V>
class HashTable {
public:
	std::vector<HashNode<K, V>*> table;
	std::vector<int> auxInfo;

	int capacity;
	int size;
	float loadFactor;

	void resize() {
		int oldCapacity = capacity;
		capacity *= 2;
		std::vector<HashNode<K, V>*> newTable(capacity);
		for (int i = 0; i < oldCapacity; i++) {
			HashNode<K, V>* head = table[i];
			while (head != nullptr) {
				K oldKey = head->key;
				V oldValue = head->value;
				HashNode<K, V>* oldNext = head->next;
				int hashIndex = hashFunction(oldKey);
				head->next = newTable[hashIndex];
				newTable[hashIndex] = head;
				head = oldNext;
			}
		}
		table = newTable;
	}

public:
	HashTable(int cap = 13, float loadFactor = 0.8) : capacity(cap), size(0), loadFactor(loadFactor) {
		table.resize(capacity);
		auxInfo.resize(capacity);
		for (int i = 0; i < capacity; i++)
			table[i] = nullptr;
	}

	~HashTable() {
		for (int i = 0; i < capacity; i++) {
			HashNode<K, V>* entry = table[i];
			while (entry != nullptr) {
				HashNode<K, V>* prev = entry;
				entry = entry->next;
				delete prev;
			}
		}
	}

	int hashFunction(K key) {
		return key % capacity; // Simple mod-based hash function
	}

	void insertNode(K key, V value) {
	/*	if ((1.0 * size) / capacity >= loadFactor) {
			resize();
		}*/
		HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
		int hashIndex = hashFunction(key);
		newNode->next = table[hashIndex];
		table[hashIndex] = newNode;
		size++;
		auxInfo[hashIndex]++;
	}

	V deleteNode(K key) {
		int hashIndex = hashFunction(key);
		HashNode<K, V>* entry = table[hashIndex];
		HashNode<K, V>* prev = nullptr;
		while (entry != nullptr && entry->key != key) {
			prev = entry;
			entry = entry->next;
		}
		if (entry == nullptr) return V(); // Key not found, return default-constructed value
		else {
			if (prev == nullptr) {
				table[hashIndex] = entry->next;
			}
			else {
				prev->next = entry->next;
			}
			V value = entry->value;
			delete entry;
			size--;
			return value;
		}
	}

	V get(K key) {
		int hashIndex = hashFunction(key);
		HashNode<K, V>* entry = table[hashIndex];
		while (entry != nullptr) {
			if (entry->key == key) return entry->value;
			entry = entry->next;
		}
		return V(); // Key not found, return default-constructed value
	}

	int getSize() {
		return size;
	}

	bool isEmpty() {
		return size == 0;
	}
};

class HashTableVisualizer : public QGraphicsView {
public:
	HashTable<int, int>* hashTable;
	QGraphicsScene* scene;

public:
	HashTableVisualizer(QWidget* parent = nullptr) : QGraphicsView(parent) {
		hashTable = new HashTable<int, int>();
		scene = new QGraphicsScene(this);
		setScene(scene);
		drawHashTable();
	}

	void wheelEvent(QWheelEvent* event)
	{
		// 当前放缩倍数;
		qreal scaleFactor = this->matrix().m11();
		int wheelDeltaValue = event->delta();
		// 向上滚动，放大;
		if (wheelDeltaValue > 0)
		{
			this->scale(1.2, 1.2);
		}
		// 向下滚动，缩小;
		else
		{
			this->scale(1.0 / 1.2, 1.0 / 1.2);
		}
	}

	~HashTableVisualizer() {
		delete hashTable;
	}

	void drawHashTable() {
		scene->clear();
		for (int i = 0; i < hashTable->table.size(); ++i) {
			HashNode<int, int>* node = hashTable->table[i];
			int x = 0;
			if (node == nullptr)
			{
				QGraphicsRectItem* rectItem = scene->addRect(x, i * 50, 40, 30);
				QGraphicsTextItem* textItem = scene->addText("^");
				textItem->setPos(x, i * 50);
				continue;
			}
			qsrand(i);
			QColor qc(qrand() % 256, qrand() % 256, qrand() % 192 + 64);
			while (node != nullptr) {
				// 创建矩形项目和文本项目
				QGraphicsTextItem* textItem = scene->addText(QString::number(node->key) + ":" + QString::number(node->value));
				textItem->setPos(x, i * 50);
				QFontMetrics fontMetrics(textItem->font());
				int textWidth = fontMetrics.width(textItem->toPlainText());
				QGraphicsRectItem* rectItem = scene->addRect(x, i * 50, textWidth + 10, 30);
				rectItem->setBrush(qc);
				textItem->setDefaultTextColor(Qt::black);
				rectItem->setZValue(-1);


				if (node->next != nullptr) {
					// 创建箭头线条
					QGraphicsLineItem* arrowLine = scene->addLine(x + textWidth + 10, i * 50 + 15, x + textWidth + 40, i * 50 + 15);
					QGraphicsPolygonItem* arrowHead = scene->addPolygon(QPolygonF() << QPointF(x + textWidth + 40, i * 50 + 15) << QPointF(x + textWidth + 30, i * 50 + 10) << QPointF(x + textWidth + 30, i * 50 + 20));
					// 设置箭头线条和箭头头部的颜色
					arrowLine->setPen(QPen(Qt::black));
					arrowHead->setBrush(Qt::black);
				}

				// 更新迭代变量
				node = node->next;
				x += textWidth + 40;
			}

		}
	}

	void insert(int key, int value) {
		hashTable->insertNode(key, value);
		drawHashTable();
	}

	void remove(int key) {
		hashTable->deleteNode(key);
		drawHashTable();
	}
};
