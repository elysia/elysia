namespace Elysia {
class Placeable;

/** Find nearest neighbor from a scatter set of locations
 */
class BRAIN_CORE_EXPORT SpatialSearch{
private:
public:
    virtual Placeable* findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude)=0;
    virtual void addNeighbor(Placeable*)=0;
    virtual void removeNeighbor(Placeable*)=0;
    virtual void moveNeighbor(Placeable*)=0;
	virtual ~SpatialSearch(){}
};
}
