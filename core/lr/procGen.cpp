#include "procGen.h"

void pushSpherePole(int start, int numSegments, ew::MeshData* data) {
	int poleStart = start;
	int sideStart = std::abs(start - (numSegments + 1));
	for (int i = 0; i < numSegments; i++) {
		data->indices.push_back(sideStart + i);
		data->indices.push_back(poleStart + i);
		data->indices.push_back(sideStart + i + 1);
	}
}

ew::MeshData lr::createSphere(float radius, int numSegments)
{
	ew::MeshData data = ew::MeshData();

	//Sets up vertices for sphere
	float thetaStep = (2.0 * ew::PI) / (1.0 * numSegments);
	ew::Vertex v;

	//Muliplied by 1.0 so no """funny events""" happen as a result of integer division
	float phiStep = ew::PI / (1.0 * numSegments);
	for (int row = 0; row <= numSegments; row++) {
		float phi = row * phiStep;
		for (int col = 0; col <= numSegments; col++) {
			float theta = col * thetaStep;
			v.pos.x = radius * cos(theta) * sin(phi);
			v.pos.y = radius * cos(phi);
			v.pos.z = radius * sin(theta) * sin(phi);
			//UVs multiplied by 0.05 so the image isn't really tiny
			v.uv = ew::Vec2(col * 0.05, row * 0.05);
			//Normals
			v.normal = ew::Normalize(v.pos);
			data.vertices.push_back(v);
		}
	}


	//Sets up indices for the poles of the sphere
	
		//Top pole 
		pushSpherePole(0, numSegments, &data);

		//All the other indices
		int columns = numSegments + 1;
		for (int row = 1; row < (numSegments + 1); row++) {
			for (int col = 0; col < numSegments; col++) {
				int start = row * columns + col;
				//Triagle 1
				data.indices.push_back(start);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns);

				//Triangle 2
				data.indices.push_back(start + columns + 1);
				data.indices.push_back(start + columns);
				data.indices.push_back(start + 1);
			}
		}
	
		//Bottom pole
		int last = data.vertices.size() - (numSegments + 1);
		pushSpherePole(last, numSegments, &data);

	return data;
}

ew::MeshData lr::createCylinder(float height, float radius, int numSegments)
{
	ew::MeshData data = ew::MeshData();
	ew::Vertex v;
	int sideStart = 0;

	//Top
	v.pos.y = height / 2.0;
	v.pos.x = 0.0;
	v.pos.z = 0.0;
	v.normal = ew::Vec3(0.0, 1.0, 0.0);
	v.uv = ew::Vec2(0.5, 0.5);
	data.vertices.push_back(v);
	sideStart++;
	//Top ring
	float thetaStep = (2 * ew::PI) / (1.0 * numSegments);
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = height / 2.0;
		v.normal = ew::Vec3(0.0,1.0,0.0);
		v.uv = ew::Vec2((radius + v.pos.x) / (radius * 2.0), (radius + v.pos.z) / (radius * 2.0));
		data.vertices.push_back(v);
		sideStart++;
	}

	//Bottom ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = -height / 2.0;
		v.normal = ew::Vec3(0.0, -1.0, 0.0);
		v.uv = ew::Vec2((radius + v.pos.x) / (radius * 2.0), (radius + v.pos.z) / (radius * 2.0));
		data.vertices.push_back(v);
		sideStart++;
	}

	//Bottom
	v.pos.y = -(height / 2.0);
	v.pos.x = 0.0;
	v.pos.z = 0.0;
	v.normal = ew::Vec3(0.0, -1.0, 0.0);
	v.uv = ew::Vec2((radius + v.pos.x) / (radius * 2.0), (radius + v.pos.z) / (radius * 2.0));
	sideStart++;
	data.vertices.push_back(v);

	//Top indices
	int start = 1;
	int center = 0;
	for (int i = 0; i < numSegments; i++) {
		data.indices.push_back(start + i);
		data.indices.push_back(center);
		data.indices.push_back(start + i + 1);
	}

	//Bottom indices
	center = (numSegments*2.0) + 3;
	start = numSegments + 2;
	for (int i = 0; i <= numSegments; i++) {
		data.indices.push_back(center);
		data.indices.push_back(start + i + 1);
		data.indices.push_back(start + i);
	}

	//Side normals

//Bottom ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = -height / 2.0;
		v.normal = ew::Normalize(v.pos - ew::Vec3(0.0, -height / 2.0, 0.0));
		v.uv = ew::Vec2(theta / (2.0 * ew::PI * radius), 0);
		data.vertices.push_back(v);
	}

	//Top ring
	for (int i = 0; i <= numSegments; i++) {
		float theta = i * thetaStep;
		v.pos.x = cos(theta) * radius;
		v.pos.z = sin(theta) * radius;
		v.pos.y = height / 2.0;
		v.normal = ew::Normalize(v.pos - ew::Vec3(0.0, height / 2.0, 0.0));
		v.uv = ew::Vec2(theta / (2.0 * ew::PI * radius), 1);
		data.vertices.push_back(v);
	}

	//Side indices
	int cols = numSegments + 1;
	for (int i = 0; i < cols; i++) {
		start = sideStart + i;
		//Triangle 1
		data.indices.push_back(start + cols);
		data.indices.push_back(start + 1);
		data.indices.push_back(start);
		
		//Triangle 2
		data.indices.push_back(start + 1);
		data.indices.push_back(start + cols);
		data.indices.push_back(start + cols + 1);
	}
	return data;
}

ew::MeshData lr::createPlane(float width, float height, int subdivisions)
{
	ew::MeshData data = ew::MeshData();
	ew::Vertex v;
	for (int row = 0; row <= subdivisions; row++) {
		for (int col = 0; col <= subdivisions; col++) {
			v.pos.x = width * (col / (subdivisions * 1.0));
			v.pos.z = -height * (row / (subdivisions * 1.0));
			v.uv = ew::Vec2(v.pos.x/width, v.pos.z/width);
			v.normal = ew::Vec3(0.0, 1.0, 0.0);
			data.vertices.push_back(v);
		}
	}

	int columns = subdivisions + 1;
	for (int row = 0; row < subdivisions; row++) {
		for (int col = 0; col < subdivisions; col++) {

			int start = row * columns + col;

			//Bottom right triangle
			data.indices.push_back(start);
			data.indices.push_back(start + 1);
			data.indices.push_back(start + columns + 1);

			//Top left triangle…
			data.indices.push_back(start);
			data.indices.push_back(start + columns + 1);
			data.indices.push_back(start + columns);
		}
	}

	return data;
}