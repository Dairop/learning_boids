#include "QuadTree.hpp"


static bool collideRectAndRect(rectByCenter rect1, rectByCenter rect2) {
    float tlx1 = rect1.center.x - rect1.radius.x;
    float tly1 = rect1.center.y - rect1.radius.y;
    float brx1 = rect1.center.x + rect1.radius.x;
    float bry1 = rect1.center.y + rect1.radius.y;

    float tlx2 = rect2.center.x - rect2.radius.x;
    float tly2 = rect2.center.y - rect2.radius.y;
    float brx2 = rect2.center.x + rect2.radius.x;
    float bry2 = rect2.center.y + rect2.radius.y;

    if ((tlx1 <= brx2) && (brx1 >= tlx2) && (tly1 <= bry2) && (bry1 >= tly2)) {
        return true;
    }
    return false;
}



void QuadTree::del() {
    points.clear();

    //if the childs are defined, then apply the function recursively
    if (northWest != nullptr) {
        northWest->del();
        northEast->del();
        southEast->del();
        southWest->del();
    }

    delete northWest;
    delete northEast;
    delete southEast;
    delete southWest;

    northWest = nullptr;
    northEast = nullptr;
    southEast = nullptr;
    southWest = nullptr;

    return;
}


void QuadTree::getAllParticles(std::vector<Entity*>& particles) {
    // Call reculrsively to gather the particles from the children
    if (northWest != nullptr) {
        northWest->getAllParticles(particles);
        northEast->getAllParticles(particles);
        southWest->getAllParticles(particles);
        southEast->getAllParticles(particles);
    }
    else {
        // If there are no children, then add our own particles
        particles.insert(particles.end(), points.begin(), points.end());
    }
}

void QuadTree::subdivide() {
    // Subdivide the previous rect in 4 equal parts
    rectByCenter r;
    r.radius = mult(boundary.radius, 0.5);
    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northWest = new QuadTree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y - boundary.radius.y / 2;
    northEast = new QuadTree(r);

    r.center.x = boundary.center.x + boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southEast = new QuadTree(r);

    r.center.x = boundary.center.x - boundary.radius.x / 2;
    r.center.y = boundary.center.y + boundary.radius.y / 2;
    southWest = new QuadTree(r);
};




bool QuadTree::insert(Entity* p) {
    // If the position of the entity is NaN, ignore it
    if (isnan(p->position.x)) return false;

    // Insert a point into the QuadTree
    // Ignore objects that do not belong to this quadtree
    if ((abs(boundary.center.x - p->position.x) > boundary.radius.x) || (abs(boundary.center.y - p->position.y) > boundary.radius.y)) {
        return false; // Object should not be added
    }

    // If there is still space in this quadtree, add the object here
    if (points.size() < QT_NODE_CAPACITY && northWest == nullptr) {
        points.push_back(p);
        return true;
    }

    // Otherwise, subdivide the quadtree and add the point to the node that will accept it
    if (northWest == nullptr) {
        subdivide();

        // Distribute the points to the child nodes
        for (int pointMoved = 0; pointMoved < points.size(); pointMoved++) {
            if (northWest->insert(points[pointMoved])) continue;
            else if (northEast->insert(points[pointMoved])) continue;
            else if (southWest->insert(points[pointMoved])) continue;
            else if (southEast->insert(points[pointMoved])) continue;
        }

        points.clear();
    }

    // Try to insert the point into the child nodes
    if (northWest->insert(p)) { return true; }
    if (northEast->insert(p)) { return true; }
    if (southWest->insert(p)) { return true; }
    if (southEast->insert(p)) { return true; }

    //If the point cannot be inserted for an unknown reason (should never happen)
    return false;
}





void QuadTree::queryRangeRect(rectByCenter range, std::vector<Entity*>& pointsInRange) {
    // Skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    // If there are no children, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->position.x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->position.y - range.center.y) < range.radius.y)) {
                pointsInRange.push_back(points.at(p));
            }
        }
        return;
    }

    // If there are children, ask them
    northWest->queryRangeRect(range, pointsInRange);
    northEast->queryRangeRect(range, pointsInRange);
    southEast->queryRangeRect(range, pointsInRange);
    southWest->queryRangeRect(range, pointsInRange);
    return;
}



// Similar to queryRangeRect but we are prunning the result a little more
void QuadTree::queryRangeCircle(rectByCenter range, std::vector<Entity*>& pointsInRange) {
    // Skip if the quadtree isn't concerned
    if (!collideRectAndRect(range, boundary)) {
        return;
    }

    // If there are children, return the node points
    if (northWest == nullptr) {
        for (int p = 0; p < points.size(); p++) {
            if ((abs(points.at(p)->position.x - range.center.x) < range.radius.x) &&
                (abs(points.at(p)->position.y - range.center.y) < range.radius.y)) {

                // In rect, then check if in circle
                if (dist2(points[p]->position, range.center) < range.radius.x * range.radius.x + range.radius.y * range.radius.y) {
                    pointsInRange.push_back(points.at(p));
                }
            }
        }
        return;
    }

    // If there are children, ask them
    northWest->queryRangeCircle(range, pointsInRange);
    northEast->queryRangeCircle(range, pointsInRange);
    southEast->queryRangeCircle(range, pointsInRange);
    southWest->queryRangeCircle(range, pointsInRange);
    return;
}