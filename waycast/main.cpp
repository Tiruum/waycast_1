#include <SFML/Graphics.hpp>
#include <cmath>

#define PI 3.14159265359

#include <vector>
#include <iostream>
#include <set>
#include <iomanip>
#include <unistd.h>
#include <Windows.h>

using namespace std;

class Map;

class Node {
private:
    int _state;
    double _cost;
    double _ev;
    int _x;
    int _y;
    Node *_parrent;
public:
    Node() {
        _state = 0;
    }

    Node(int state, int x, int y) {
        _parrent = nullptr;
        _state = state;
        _x = x;
        _y = y;
        _ev = 0;
        _cost = 0;
    }

    friend Map;
};

class Map {
private:
    vector<vector<Node *> *> _nodes;
    int _size_x;
    int _size_y;
    int _start_x;
    int _start_y;
    int _end_x;
    int _end_y;

public:
    Map(int size_x, int size_y) {
        _size_x = size_x;
        _size_y = size_y;
        for (int i = 0; i < _size_y; i++) {
            _nodes.push_back(new vector<Node *>);
            for (int j = 0; j < _size_x; j++) {
                _nodes.at(i)->push_back(new Node(0, j, i));
            }
        }
    }

    int get_state(int x, int y) {
        if (x < 0 || x >= _size_x || y < 0 || y >= _size_y) {
            return -1;
        }
        return _nodes.at(y)->at(x)->_state;
    }

    int get_cost(int x, int y) {
        if (x < 0 || x >= _size_x || y < 0 || y >= _size_y) {
            return -1;
        }
        return _nodes.at(y)->at(x)->_cost;
    }

    void set_state(int x, int y, int state) {
        if (x < 0 || x >= _size_x || y < 0 || y >= _size_y) {
            return;
        }
        _nodes.at(y)->at(x)->_state = state;

    }

    void clear() {
        for (int i = 0; i < _size_y; i++) {
            for (int j = 0; j < _size_x; j++) {
                _nodes.at(i)->at(j)->_state = 0;
            }
        }
    }

    void set_start(int x, int y) {
        _start_x = x;
        _start_y = y;
    }

    void set_end(int x, int y) {
        _end_x = x;
        _end_y = y;
    }

    double get_dist(Node *current) {
        return abs(current->_x - _end_x) + abs(current->_y - _end_y);
    }

    bool BFS() {

        set<Node *> _opened;
        set<Node *> _closed;
        set<Node *> _neighbours;
        Node *_neighbour;
        double score;
        int min_ev = -1;
        for (int i = 0; i < _size_y; i++) {
            for (int j = 0; j < _size_x; j++) {
                _nodes.at(i)->at(j)->_parrent = nullptr;
            }
        }
        Node *_current = _nodes.at(_start_y)->at(_start_x);
        _opened.insert(_current);
        _current->_cost = 0;
        _current->_ev = 0; // get_dist(_current);

        while (_opened.size() > 0) {

            for (auto item: _opened) {
                if (min_ev == -1) {
                    min_ev = item->_ev;
                    _current = item;

                } else if (item->_ev < min_ev) {
                    min_ev = item->_ev;
                    _current = item;
                }
            }
            min_ev = -1;
            if (_current == _nodes.at(_end_y)->at(_end_x)) {
                return true;
            }
            _opened.erase(_current);
            _closed.insert(_current);
            _neighbours.clear();
            if (_current->_x > 0) {
                _neighbour = _nodes.at(_current->_y)->at(_current->_x - 1);
                _neighbours.insert(_neighbour);
            }

            if (_current->_x < _size_x - 1) {
                _neighbour = _nodes.at(_current->_y)->at(_current->_x + 1);
                _neighbours.insert(_neighbour);
            }
            if (_current->_y > 0) {
                _neighbour = _nodes.at(_current->_y - 1)->at(_current->_x);
                _neighbours.insert(_neighbour);
            }
            if (_current->_y < _size_y - 1) {
                _neighbour = _nodes.at(_current->_y + 1)->at(_current->_x);
                _neighbours.insert(_neighbour);
            }


            for (auto item: _neighbours) {
                score = _current->_cost + 1;
                if (_closed.find(item) != _closed.end() && score >= item->_cost || item->_state == 1) {
                    continue;

                }
                if ((_closed.find(item) == _closed.end()) || score < item->_cost) {
                    item->_parrent = _current;
                    item->_cost = score;
                    item->_ev = item->_cost; // +get_dist(item);
                    if (_opened.find(item) == _opened.end()) {
                        _opened.insert(item);
                    }
                }

            }


        }
        return false;
    }

    vector<pair<int, int>> get_way() {
        vector<pair<int, int>> way;
        Node *_current = _nodes.at(_end_y)->at(_end_x);
        bool check;

        while (_current->_parrent) {
            way.push_back(make_pair(_current->_x, _current->_y));
            cout << _current->_x << " " << _current->_y << endl;
            _current = _current->_parrent;
        }
        way.push_back(make_pair(_current->_x, _current->_y));
        return way;

    }

    void print() {
        cout << endl;
        for (int i = 0; i < _size_y; i++) {
            for (int j = 0; j < _size_x; j++) {
                cout << setw(2) << _nodes.at(i)->at(j)->_ev << " ";
            }
            cout << endl;
        }
    }
};

void
draw_line(sf::RenderWindow *window, double x1, double y1, double x2, double y2, double thickness, sf::Color color) {
    sf::ConvexShape d_line;
    d_line.setPointCount(4);
    d_line.setFillColor(color);
    double a = -atan((x2 - x1) / (y2 - y1));

    d_line.setPoint(0, sf::Vector2f(x1 + thickness * cos(a + PI) / 2, y1 + thickness * sin(a + PI) / 2));
    d_line.setPoint(1, sf::Vector2f(x1 + thickness * cos(a + 2 * PI) / 2, y1 + thickness * sin(a + 2 * PI) / 2));
    d_line.setPoint(2, sf::Vector2f(x2 + thickness * cos(a + 2 * PI) / 2, y2 + thickness * sin(a + 2 * PI) / 2));
    d_line.setPoint(3, sf::Vector2f(x2 + thickness * cos(a + PI) / 2, y2 + thickness * sin(a + PI) / 2));
    window->draw(d_line);
}

int main() {
    int m_x, m_y, mcp_x, mcp_y;
    double scr_X = 800;
    double scr_Y = 800;
    double min_side;
    double thickness = 1;
    double way_thickness = 4;
    double cell_count = 100;
    double cell_size;
    int start_x = 0;
    int start_y = 0;
    int end_x = cell_count - 1;
    int end_y = cell_count - 1;
    int check = 0;
    bool flag = true;
    vector<pair<int, int>> way;
    vector<pair<int, int>>::iterator it;
    bool found = false;
    sf::RectangleShape rect;
    Map mmap(cell_count, cell_count);
    mmap.set_start(start_x, start_y);
    mmap.set_end(end_x, end_y);


    if (scr_X > scr_Y) {
        cell_size = (scr_Y - thickness * (cell_count + 1)) / cell_count;
        min_side = scr_Y;
    } else {
        cell_size = (scr_X - thickness * (cell_count + 1)) / cell_count;
        min_side = scr_X;
    }
    rect.setFillColor(sf::Color::Black);
    rect.setSize(sf::Vector2f(cell_size, cell_size));

    sf::RenderWindow window(sf::VideoMode(scr_X, scr_Y), "Way");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);


        m_x = sf::Mouse::getPosition(window).x;
        m_y = sf::Mouse::getPosition(window).y;

        mcp_x = (int) ((m_x - thickness * 2) / (cell_size + thickness));
        mcp_y = (int) ((m_y - thickness * 2) / (cell_size + thickness));

        if (mcp_x > cell_count - 1) {
            mcp_x = cell_count - 1;
        }
        if (mcp_y > cell_count - 1) {
            mcp_y = cell_count - 1;
        }
        if (mcp_x < 0) {
            mcp_x = 0;
        }
        if (mcp_y < 0) {
            mcp_y = 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!(mcp_x == start_x && mcp_y == start_y || mcp_x == end_x && mcp_y == end_y)) {
                mmap.set_state(mcp_x, mcp_y, 1);
                flag = true;
                found = false;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            if (!(mcp_x == start_x && mcp_y == start_y || mcp_x == end_x && mcp_y == end_y)) {
                mmap.set_state(mcp_x, mcp_y, 0);
                flag = true;
                found = false;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            if (!(mcp_x == end_x && mcp_y == end_y)) {
                mmap.set_state(mcp_x, mcp_y, 0);
                start_x = mcp_x;
                start_y = mcp_y;
                mmap.set_start(start_x, start_y);
                flag = true;
                found = false;
            }

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (!(mcp_x == start_x && mcp_y == start_y)) {
                mmap.set_state(mcp_x, mcp_y, 0);
                end_x = mcp_x;
                end_y = mcp_y;
                mmap.set_end(end_x, end_y);
                flag = true;
                found = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && flag) {
            found = mmap.BFS();
            if (found) {
                way = mmap.get_way();
                cout << "Way found" << endl;
                check = 1;
            } else {
                cout << "Way not found" << endl;
                check = 2;
            }
            cout << found;
            mmap.print();
            flag = false;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            mmap.clear();
            flag = true;
        }

        for (double i = thickness / 2; i <= min_side - thickness / 2 + 1; i += cell_size + thickness) {
            draw_line(&window, i, 0, i, min_side, thickness, sf::Color::Black);
        }
        for (double i = thickness / 2; i <= min_side - thickness / 2 + 1; i += cell_size + thickness) {
            draw_line(&window, 0, i, min_side, i, thickness, sf::Color::Black);
        }
        for (int i = 0; i < (int) cell_count; i++) {
            for (int j = 0; j < (int) cell_count; j++) {
                if (mmap.get_state(i, j) == 1) {
                    rect.setPosition(
                            sf::Vector2f(cell_size * i + thickness * (i + 1), cell_size * j + thickness * (j + 1)));
                    window.draw(rect);

                }
            }
        }
        if (found) {
            for (it = way.begin() + 1; it != way.end(); ++it) {
                draw_line(&window,
                          cell_size * (it - 1)->first + thickness * ((it - 1)->first + 1) + cell_size / 2,
                          cell_size * (it - 1)->second + thickness * ((it - 1)->second + 1) + cell_size / 2,
                          cell_size * it->first + thickness * (it->first + 1) + cell_size / 2,
                          cell_size * it->second + thickness * (it->second + 1) + cell_size / 2,
                          way_thickness, sf::Color::Magenta);

            }
        }
        if (check == 1) {
            sf::Font font;
            font.loadFromFile("C:/Users/Timur/Desktop/bahnschrift.ttf");
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setString("Way found");
            text.setPosition(600, 0);
            text.setFillColor(sf::Color::Green);
            window.draw(text);
        } else if ((check == 2)) {
            sf::Font font;
            font.loadFromFile("C:/Users/Timur/Desktop/bahnschrift.ttf");
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setString("Way not found");
            text.setPosition(600, 0);
            text.setFillColor(sf::Color::Red);
            window.draw(text);
        }

        rect.setPosition(sf::Vector2f(cell_size * start_x + thickness * (start_x + 1),
                                      cell_size * start_y + thickness * (start_y + 1)));
        rect.setFillColor(sf::Color::Green);
        window.draw(rect);
        rect.setPosition(
                sf::Vector2f(cell_size * end_x + thickness * (end_x + 1), cell_size * end_y + thickness * (end_y + 1)));
        rect.setFillColor(sf::Color::Red);
        window.draw(rect);
        rect.setFillColor(sf::Color::Black);
        window.display();
    }
    return 0;
}
