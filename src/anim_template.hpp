#ifndef ANIM_TEMPLATE_HPP
#define ANIM_TEMPLATE_HPP

struct AnimTemplate {
    AnimTemplate(int frame_width, int frame_height, int anim_length = 1, int y_index = 0, double fps = 10) :
        m_frame_width(frame_width),
        m_frame_height(frame_height),
        m_anim_length(anim_length),
        m_y_index(y_index),
        m_fps(fps) {

    }

    int length() {
        return m_anim_length;
    }

    int y_index() {
        return m_y_index;
    }

    int frame_width() {
        return m_frame_width;
    }

    int frame_height() {
        return m_frame_height;
    }

    double fps() {
        return m_fps;
    };
  private:
    int m_frame_width;
    int m_frame_height;
    int m_anim_length;
    int m_y_index;
    double m_fps;
};

#endif
