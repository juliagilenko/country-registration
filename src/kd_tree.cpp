#include "kd_tree.h"

KD_Tree::KD_Tree(Country* country)
{
    std::vector<Line*> lines;
    for (Eigen::MatrixXd landmass : country->get_landmasses())
    {
        for (int i = 0; i < landmass.cols() - 1; i++)
        {
            lines.push_back(new Line(landmass.col(i), landmass.col(i + 1)));
        }
    }

    root = build_tree(lines, 0);
}

KD_Tree::~KD_Tree()
{
    destruct(root);
}

KD_Tree::Node* KD_Tree::build_tree(std::vector<Line*> lines, int depth)
{
    if (lines.empty())
    {
        return NULL;
    }

    int axis = depth % 2;

    std::sort(
        lines.begin(),
        lines.end(),
        [axis](const Line* l1, const Line* l2) -> bool { return l1->get_centre()[axis] < l2->get_centre()[axis]; });

    int median = lines.size() / 2;
    std::vector<Line*> left_children = median == 0 ? std::vector<Line*>() : std::vector<Line*>(lines.begin(), lines.begin() + median);
    std::vector<Line*> right_children = median + 1 > lines.size() ? std::vector<Line*>() : std::vector<Line*>(lines.begin() + median + 1, lines.end());
    return new Node(lines.at(median), depth, build_tree(left_children, depth + 1), build_tree(right_children, depth + 1));
}

Eigen::Vector2d KD_Tree::find_closest_point(Eigen::Vector2d point) const
{
    double dist = INFINITY;
    Eigen::Vector2d closest_point;
    find_closest_point(point, root, closest_point, dist);
    return closest_point;
}

void KD_Tree::find_closest_point(Eigen::Vector2d query_point, Node* cur_node, Eigen::Vector2d& closest_point, double& dist) const
{
    double cur_dist;
    Eigen::Vector2d cur_closest_point;

    if (cur_node->is_leaf())
    {
        cur_node->get_line()->compute_closest_point(query_point, cur_closest_point, cur_dist);
        if (cur_dist < dist)
        {
            closest_point = cur_closest_point;
            dist = cur_dist;
        }
        return;
    }

    int axis = cur_node->get_depth() % 2;
    bool chose_right;
    if (query_point[axis] < cur_node->get_line()->get_centre()[axis])
    {
        // go left for smaller values to reach leaf node (unless there's no left child)
        if (cur_node->get_left_child())
        {
            find_closest_point(query_point, cur_node->get_left_child(), closest_point, dist);
            chose_right = false;
        }
        else
        {
            find_closest_point(query_point, cur_node->get_right_child(), closest_point, dist);
            chose_right = true;
        }
    }
    else
    {
        // go right for larger values to reach leaf node (unless there's no right child)
        if (cur_node->get_right_child())
        {
            find_closest_point(query_point, cur_node->get_right_child(), closest_point, dist);
            chose_right = true;
        }
        else
        {
            find_closest_point(query_point, cur_node->get_left_child(), closest_point, dist);
            chose_right = false;
        }
    }

    // if parent is closer than child, check parent's other branch
    cur_node->get_line()->compute_closest_point(query_point, cur_closest_point, cur_dist);
    if (cur_dist < dist)
    {
        closest_point = cur_closest_point;
        dist = cur_dist;

        if (chose_right && cur_node->get_left_child())
        {
            find_closest_point(query_point, cur_node->get_left_child(), closest_point, dist);
        }
        else if (!chose_right && cur_node->get_right_child())
        {
            find_closest_point(query_point, cur_node->get_right_child(), closest_point, dist);
        }
    }

    return;
}

void KD_Tree::destruct(Node* node) const
{
    if (node->is_leaf())
    {
        delete(node->get_line());
        delete(node);
        return;
    }

    if (node->get_right_child())
    {
        destruct(node->get_right_child());
        node->remove_right_child();
    }
    if (node->get_left_child())
    {
        destruct(node->get_left_child());
        node->remove_left_child();
    }

    delete(node->get_line());
    delete(node);
}

KD_Tree::Node::Node(Line* line, int depth, Node* left_child, Node* right_child)
{
    this->line = line;
    this->depth = depth;
    this->left_child = left_child;
    this->right_child = right_child;
}

Line* KD_Tree::Node::get_line() const
{
    return line;
}

int KD_Tree::Node::get_depth() const
{
    return depth;
}

KD_Tree::Node* KD_Tree::Node::get_right_child() const
{
    return right_child;
}

KD_Tree::Node* KD_Tree::Node::get_left_child() const
{
    return left_child;
}

void KD_Tree::Node::remove_right_child() 
{
    right_child = NULL;
}

void KD_Tree::Node::remove_left_child()
{
    left_child = NULL;
}

bool KD_Tree::Node::is_leaf() const
{
    return !right_child && !left_child;
}