#ifndef PATH_DEFINITION
#define PATH_DEFINITION

enum Path_Type {PARABLE, NORMAL};

struct Path_Def {
	Path_Def(std::function<glm::vec2(entityx::Entity)> path_function)
		: m_path_type(NORMAL), m_path_function(path_function){

	}

	Path_Def(std::function<glm::vec2(float)> parable_function, glm::vec2 origin, float direction)
		: m_path_type(PARABLE),
		m_parable_function(parable_function),
		m_origin(origin),
		m_direction(direction){

	}

	Path_Type get_path_type(){
		return m_path_type;
	}

	auto get_path_function(){
		return m_path_function;
	}

	auto get_parable_function(){
		return m_parable_function;
	}

	glm::vec2 get_origin(){
		return m_origin;
	}

	float get_direction(){
		return m_direction;
	}

	private:
		Path_Type m_path_type;

		std::function<glm::vec2(entityx::Entity)> m_path_function;

		std::function<glm::vec2(float)> m_parable_function;
		glm::vec2 m_origin;
		float m_direction;
};

#endif
