class Elevator
{
private:
	int elvtrPos;
	int elvtrDir;
public:
	Elevator(int elvtrPos, int elvtrDir);
	void setPosition(int elvtrPos);
	void setDirection(int elvtrDir);
	int getPosition();
	int getDirection();
};
Elevator::Elevator(int elvtrPos, int elvtrDir){
	this->elvtrPos = elvtrPos;
	this->elvtrDir = elvtrDir;
}
void Elevator::setPosition(int elvtrPos) {
	this->elvtrPos = elvtrPos;
}
void Elevator::setDirection(int elvtrDir) {
	this->elvtrDir = elvtrDir;
}
int Elevator::getPosition(){
	return elvtrPos;
}
int Elevator::getDirection() {
	return elvtrDir;
}