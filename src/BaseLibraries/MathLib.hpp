#include <algorithm>
#include <cmath>

#define PI 3.14159265

double shiftIntoDegreePeriod(double degrees)
{
    double period = 360;
    int periods = degrees / period;
    double ret = degrees - (periods * period);
    if (ret < 0)
    {
        return ret + period;
    }
    return ret;
}

double shiftIntoRadianPeriod(double radians)
{
    double period = 2 * PI;
    int periods = radians / period;
    double ret = radians - (periods * period);
    if (ret < 0)
    {
        return ret + period;
    }
    return ret;
}

double toDegrees(double radians)
{
    return shiftIntoDegreePeriod(radians * 180 / PI);
}

double toRadians(double degrees)
{
    return shiftIntoRadianPeriod(degrees * PI / 180);
}

struct Point
{
    double x;
    double y;
    double z;

    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

double distanceBetweenPoints(Point a, Point b)
{
    double opp = abs(a.y - b.y);
    double adj = abs(a.x - b.x);
    double hyp = sqrt(pow(opp, 2) + pow(adj, 2));
    adj = hyp;
    opp = abs(a.z - b.z);
    hyp = sqrt(pow(opp, 2) + pow(adj, 2));
    return hyp;
}

void degreeAngleBetweenPoints(Point a, Point b, double &horizontal)
{
    Point sw = a;
    a = b;
    b = sw;
    double opp = abs(a.y - b.y);
    double adj = abs(a.x - b.x);
    double degrees = toDegrees(atan(opp/adj));
    if (a.y >= b.y && a.x > b.x)
    {
        horizontal = degrees;
    }
    else if (a.y > b.y && a.x <= b.x)
    {
        horizontal = 180 - degrees;
    }
    else if (a.y <= b.y && a.x < b.x)
    {
        horizontal = 180 + degrees;
    }
    else
    {
        horizontal = 360 - degrees;
    }
}

void degreeAngleBetweenPoints(Point a, Point b, double &horizontal, double &vertical)
{
    Point sw = a;
    a = b;
    b = sw;
    double opp = abs(a.y - b.y);
    double adj = abs(a.x - b.x);
    double hyp = sqrt(pow(opp, 2) + pow(adj, 2));
    double degrees = toDegrees(atan(opp/adj));
    if (a.y >= b.y && a.x > b.x)
    {
        horizontal = degrees;
    }
    else if (a.y > b.y && a.x <= b.x)
    {
        horizontal = 180 - degrees;
    }
    else if (a.y <= b.y && a.x < b.x)
    {
        horizontal = 180 + degrees;
    }
    else
    {
        horizontal = 360 - degrees;
    }
    adj = hyp;
    opp = abs(a.z - b.z);
    degrees = toDegrees(atan(opp/adj));
    if (a.z >= b.z)
    {
        vertical = 90 + degrees;
    }
    else if (a.z < b.z)
    {
        vertical = 90 - degrees;
    }
}

void radianAngleBetweenPoints(Point a, Point b, double &horizontal)
{
    Point sw = a;
    a = b;
    b = sw;
    double opp = abs(a.y - b.y);
    double adj = abs(a.x - b.x);
    double radians = atan(opp/adj);
    if (a.y >= b.y && a.x > b.x)
    {
        horizontal = radians;
    }
    else if (a.y > b.y && a.x <= b.x)
    {
        horizontal = 180 - radians;
    }
    else if (a.y <= b.y && a.x < b.x)
    {
        horizontal = 180 + radians;
    }
    else
    {
        horizontal = 360 - radians;
    }
}

void radianAngleBetweenPoints(Point a, Point b, double &horizontal, double &vertical)
{
    Point sw = a;
    a = b;
    b = sw;
    double opp = abs(a.y - b.y);
    double adj = abs(a.x - b.x);
    double hyp = sqrt(pow(opp, 2) + pow(adj, 2));
    double radians = atan(opp/adj);
    if (a.y >= b.y && a.x > b.x)
    {
        horizontal = radians;
    }
    else if (a.y > b.y && a.x <= b.x)
    {
        horizontal = 180 - radians;
    }
    else if (a.y <= b.y && a.x < b.x)
    {
        horizontal = 180 + radians;
    }
    else
    {
        horizontal = 360 - radians;
    }
    adj = hyp;
    opp = abs(a.z - b.z);
    radians = atan(opp/adj);
    if (a.z < b.z)
    {
        vertical = radians;
    }
    else if (a.z >= b.z)
    {
        vertical = 180 - radians;
    }
}

Point calculateVector(Point start, double length, double horizontalDegrees, double verticalDegrees = 90)
{
    if (length == 0)
    {
        return start;
    }
    Point target;
    verticalDegrees = verticalDegrees - 90;
    target.z = start.z + length * sin(toRadians(verticalDegrees));
    double hyp = length * cos(toRadians(verticalDegrees));
    target.y = start.y + hyp * sin(toRadians(horizontalDegrees));
    target.x = start.x + hyp * cos(toRadians(horizontalDegrees));
    return target;
}

struct BoundingCube
{
    Point top;
    Point bottom;
    Point front;
    Point back;
    Point left;
    Point right;
    Point center;
    double proximity;
};

int random(int max, int min = 0)
{
    max -= min;
    return rand() % max + min;
}
