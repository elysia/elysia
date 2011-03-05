

namespace Elysia {
class Brain;

class BRAIN_CORE_EXPORT Placeable {
private:
    ///Neuron location
    Vector3f mNeuronLocation;
  protected:
    Brain*mBrain;

  public:
	Brain* getBrain() const {return mBrain;}

    const Vector3f& getLocation()const {return mNeuronLocation;}
    Placeable (const Vector3f&location);
    ~Placeable();
};
}
