//Takes care of disconnecting DendriteTip. Should be called in destructor
int Synapse::detach(){
    mRecipient->removeSynapse(this);
    *mRecipient = NULL;
}