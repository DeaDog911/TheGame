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

void saveLevel(int lvl) {
    ofstream out(saves_path, ios::out);
    if (out.is_open()) {
        string save_str = "level: " + to_string(lvl);
        out.write(save_str.c_str(), sizeof(save_str.c_str()) + 1);
        out.close();
    }
}

int getSaveLevel() {
    ifstream in(saves_path, ios::in);
    if (in.is_open()) {
        char buff[20];
        in.read(buff, sizeof(buff));
        in.close();
        return getLevelFromSave(buff);
    }
    return -1;
}