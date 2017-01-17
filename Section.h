#ifndef SECTION_H
#define SECTION_H

class Section {
public:

	
	Section(int section_ends, int number_of_enemies) : 
		section_ends(section_ends), number_of_enemies(number_of_enemies) 
	{

	}

	~Section() {

	}

	bool IsFinished() const {
		return number_of_enemies == 0;
	}

	void EnemyKilled() {
		--number_of_enemies;
	}

	int getNumberOfEnemies() const {
		return number_of_enemies;
	}

public:
	int section_ends = 0;
private:
	int number_of_enemies = 0;

};

#endif // !SECTION_H

