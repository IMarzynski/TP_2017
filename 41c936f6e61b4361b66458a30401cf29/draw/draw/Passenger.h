class Passenger
{
private:
	int psngrPosX;
	int psngrPosY;
	int psngrFloor;
	int destFloor;
public:
	Passenger(int psngrPosY, int psngrFloor, int destFloor);
	Passenger();
	void setPositionX(int psngrPosX);
	void setPositionY(int psngrPosY);
	int getPositionX();
	int getPositionY();
	int getFloor();
	int getDestFloor();
};
Passenger::Passenger() {

}
Passenger::Passenger(int psngrPosY, int psngrFloor, int destFloor) {
	this->psngrPosY = psngrPosY;
	this->psngrFloor = psngrFloor;
	this->destFloor = destFloor;
}
void Passenger::setPositionX(int psngrPosX) {
	this->psngrPosX = psngrPosX;
}
void Passenger::setPositionY(int psngrPosY) {
	this->psngrPosY = psngrPosY;
}
int Passenger::getPositionX() {
	return psngrPosX;
}
int Passenger::getPositionY() {
	return psngrPosY;
}
int Passenger::getFloor() {
	return psngrFloor;
}
int Passenger::getDestFloor() {
	return destFloor;
}