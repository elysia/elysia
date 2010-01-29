namespace Elysia {
class Neuron;

/** Find nearest neighbor from a scatter set of locations
 */
class SpatialSearch{
private:
public:
    virtual Neuron* findNearestNeighbor(const Vector3f &queryPoint)=0;
    virtual void addNeighbor(Neuron*)=0;
    virtual void removeNeighbor(Neuron*)=0;
    virtual void moveNeighbor(Neuron*)=0;
};
}
