#pragma once

class Tree;
class Chamaecyparis;
class Fir;
class Terrain;
class Palm;
class Lumberjack;
class Grass;

class Visitor
{
public:
	virtual void Visit(Tree* tree) = 0;
	virtual void Visit(Chamaecyparis* white_tree) = 0;
	virtual void Visit(Fir* fir) = 0;
	virtual void Visit(Terrain* terrain) = 0;
	virtual void Visit(Palm* palm) = 0;
	virtual void Visit(Lumberjack* lumberjack) = 0;
	virtual void Visit(Grass* grass) = 0;
};