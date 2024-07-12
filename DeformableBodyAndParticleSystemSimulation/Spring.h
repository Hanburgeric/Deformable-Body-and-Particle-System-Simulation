#ifndef SPRING_H
#define SPRING_H

class Node;

class Spring
{
public:
	Spring(Node* pNode1, Node* pNode2, const float& spring, const float& damping);

	void applyForce();

	void render();

	Node* m_pNode1;
	Node* m_pNode2;
	float m_spring;
	float m_damping;
	float m_initLength;
};

#endif
