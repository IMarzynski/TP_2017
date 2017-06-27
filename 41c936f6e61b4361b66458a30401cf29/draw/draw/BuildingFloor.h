class BuildingFloor
{
private:
	int placeToStop;
	int psngrAmount;
	
public:
	BuildingFloor(int placeToStop);
	BuildingFloor();
	int getFloorStop();
	int getPsngrAmount();
	void incrPsngrAmount();
	void decrPsngrAmount();
};
BuildingFloor::BuildingFloor(int placeToStop){
	this->placeToStop = placeToStop;
	this->psngrAmount = 0; 
}
BuildingFloor::BuildingFloor(){

}
int BuildingFloor::getFloorStop(){
	return placeToStop;
}
void BuildingFloor::incrPsngrAmount() {
	this->psngrAmount++;
}
void BuildingFloor::decrPsngrAmount() {
	this->psngrAmount--;
}
int BuildingFloor::getPsngrAmount() {
	return psngrAmount;
}