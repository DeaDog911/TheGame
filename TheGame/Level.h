#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include "SFMLOrthogonalLayer.h"

using namespace tmx;
using namespace std;

class Level {
public:
	Level() {}
	Map map;
	Level(string path) {
		map.load(path);
	}
	const auto& GetObject(string name) {
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				for (const auto& object : objects)
				{
					if (object.getName() == name) {
						return object;
					}
				}
			}

		}
	}

	vector<Object> GetAllObjects() {
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				return layer->getLayerAs<tmx::ObjectGroup>().getObjects();
			}

		}
	}

	vector<Object> GetObjects(string name) {
		vector<tmx::Object> obj;
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				for (const auto& object : objects)
				{
					if (object.getName() == name) {
						obj.push_back(object);
					}
				}
			}

		}
		return obj;
	}

	vector<Object> GetObjectsWithType(string type) {
		vector<tmx::Object> obj;
		const auto& layers = map.getLayers();
		for (const auto& layer : layers)
		{
			if (layer->getType() == tmx::Layer::Type::Object)
			{
				const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
				for (const auto& object : objects)
				{
					if (object.getType() == type) {
						obj.push_back(object);
					}
				}
			}

		}
		return obj;
	}
};