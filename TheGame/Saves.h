#pragma once

int getLevelFromSave(string str) {
    int lvl_s = -1, lvl_e = -1;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            if (lvl_s == -1)
                lvl_s = i;
        }
        else {
            if (lvl_s != -1) {
                lvl_e = i;
                break;
            }
        }
    }
    string lvl_str = str.substr(lvl_s - 1, lvl_e);
    return atoi(lvl_str.c_str());
}

void saveLevel(int lvl, string weapon) {
    ofstream out(saves_path, ios::out);
    if (out.is_open()) {
        string save_str = "level: " + to_string(lvl);
        save_str.append("\n");
        save_str.append(weapon.c_str());
        out.write(save_str.c_str(), save_str.size());
        out.close();
    }
}

int getSaveLevel() {
    ifstream in(saves_path, ios::in);
    string line;
    int lvl = -1;
    if (in.is_open()) {
        getline(in, line);
        lvl = getLevelFromSave(line);
    }
    in.close();
    return lvl;
}

string getSaveWeapon() {
    ifstream in(saves_path, ios::in);
    string line;
    if (in.is_open()) {
        getline(in, line);
        getline(in, line);
        in.close();
        return line;
    }
    return "default";
}