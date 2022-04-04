#ifndef KD_TREE_H
#define KD_TREE_H

#include "country.h"
#include "line.h"

/*
* A k-d tree built on the line segments of a country's borders.
*/
class KD_Tree
{
public:
    KD_Tree(Country* country);
    Eigen::Vector2d find_closest_point(Eigen::Vector2d point) const;
    ~KD_Tree();

private:
    class Node
    {
    public:
        Node(Line* line, int depth, Node* left_child, Node* right_child);
        Line* get_line() const;
        int get_depth() const;
        Node* get_right_child() const;
        Node* get_left_child() const;
        void remove_right_child();
        void remove_left_child();
        bool is_leaf() const;

    private:
        Line* line;
        int depth;
        Node* right_child;
        Node* left_child;
    };

    Node* root;
    Node* build_tree(std::vector<Line*> lines, int depth);
    void find_closest_point(Eigen::Vector2d query_point, Node* cur_node, Eigen::Vector2d& closest_point, double& shortest_dist) const;
    void destruct(Node* node) const;
};

#endif