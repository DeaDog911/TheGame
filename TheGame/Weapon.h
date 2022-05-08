#pragma once


#include "Item.h"

enum WeaponType { Melee, Gun, None };
enum WeaponName {Pistol, Shotgun, Rifle, Bat, Pipe};

WeaponName getEnumIndex(string name) {
	if (name == "shotgun") return Shotgun;
	if (name == "pistol") return Pistol;
	if (name == "rifle") return Rifle;
	if (name == "bat") return Bat;
	if (name == "pipe") return Pipe;
}

sf::IntRect getWeaponTextureRect(WeaponName name) {
	switch (name) {
		case Pistol: return sf::IntRect(444, 22, 13, 6);
		case Shotgun: return sf::IntRect(74, 22, 27, 9);
		case Rifle:	return sf::IntRect(14, 20, 31, 11);
		case Bat: return sf::IntRect(260, 22, 21, 4);
		case Pipe: return sf::IntRect(134, 21, 27, 4);
	}
}

string getPlayerSpriteFile(WeaponName name) {
	switch (name) {
	case Shotgun: return "images/player_shotgun.png";
	case Pipe: return "images/player_pipe.png";
	case Bat: return "images/player_bat.png";
	case Rifle: return "images/player_rifle.png";
	case Pistol: return "images/player_pistol.png";
	}
}

sf::IntRect getPlayerTextureRect(WeaponName name) {
	switch (name) {
	case Shotgun: return sf::IntRect(90, 8, 32, 18);
	case Pipe: return sf::IntRect(82, 23, 16, 31);
	case Bat: return sf::IntRect(82, 23, 16, 31);
	case Rifle: return sf::IntRect(49, 8, 29, 17);
	case Pistol: return sf::IntRect(89, 6, 35, 13);
	}
}

int getSoundId(WeaponName name) {
	switch (name) {
		case Rifle: return 0;
		case Shotgun: return 1;
		case Pistol: return 2;
	}
}

class Weapon : public Item {
private:
	void setDamageAndAmmo() {
		switch (name) {
		case Shotgun: damage = 50; ammoCount = 12; maxAmmoCount = 12; break;
		case Rifle: damage = 40; ammoCount = 20; maxAmmoCount = 20; break;
		case Pistol: damage = 30; ammoCount = 30; maxAmmoCount = 30; break;
		case Pipe: damage = 25; ammoCount = 0; maxAmmoCount = 0; break;
		case Bat: damage = 20; ammoCount = 0; maxAmmoCount = 0; break;
		}
	}
public:
	float damage;
	bool active;
	int ammoCount;
	int maxAmmoCount;
	WeaponType type;
	WeaponName name;
	Weapon() { active = false; this->type = None; }
	Weapon(Texture & texture, WeaponType type, WeaponName name) : Item() {
		this->name = name;
		this->type = type;
		active = false;
		sprite.setTexture(texture);
		sprite.setTextureRect(getWeaponTextureRect(name));
		width = sprite.getTextureRect().width;
		height = sprite.getTextureRect().height;
		sprite.setScale(2, 2);
		width *= 2;
		height *= 2;

		setDamageAndAmmo();
	}
};