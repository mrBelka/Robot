#include <chrono>
#include <functional>
#include <memory>
#include <mutex>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "telerobot_interfaces/msg/motor.hpp"

#include <ModbusMaster.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;

class WheelDriver : public rclcpp::Node
{
  public:
    WheelDriver() : Node("wheel_driver")
    {
        m_modbus = std::make_unique<robot::protocol::ModbusMaster>("/dev/ttyUSB0", 115200);
        m_modbus->Setup();

        m_encoders_pub = this->create_publisher<telerobot_interfaces::msg::Motor>("encodes", 10);
        m_encoders_timer = this->create_wall_timer(200ms, std::bind(&WheelDriver::encoders_callback, this));

        m_wheel_commands_sub = this->create_subscription<telerobot_interfaces::msg::Motor>(
                "wheel_commands", 10, std::bind(&WheelDriver::wheel_commands, this, _1));
    }

  private:
    void wheel_commands(const telerobot_interfaces::msg::Motor & msg)
    {
        RCLCPP_INFO(this->get_logger(), "%d", msg.motor_lf);
        RCLCPP_INFO(this->get_logger(), "%d", msg.motor_rf);
        RCLCPP_INFO(this->get_logger(), "%d", msg.motor_lr);
        RCLCPP_INFO(this->get_logger(), "%d", msg.motor_rr);

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_modbus->WriteMultiAnalogOutput(0x01, 0x0001,{
                    static_cast<uint16_t>(msg.motor_lf + 255),
                    static_cast<uint16_t>(msg.motor_rf + 255),
                    static_cast<uint16_t>(msg.motor_lr + 255),
                    static_cast<uint16_t>(msg.motor_rr + 255)
            });
        }

    }

    void encoders_callback()
    {
        auto encoders_msg = telerobot_interfaces::msg::Motor();

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::vector<uint16_t> data = m_modbus->ReadAnalogInput(0x01, 0x0001, 8);

            for (auto d: data)
                RCLCPP_INFO(this->get_logger(), "%d", d);
            encoders_msg.motor_lf = data[0];
            encoders_msg.motor_lf = data[1];
            encoders_msg.motor_lf = data[2];
            encoders_msg.motor_lf = data[3];
        }

        m_encoders_pub->publish(encoders_msg);
    }

    std::mutex m_mutex;
    std::unique_ptr<robot::protocol::ModbusMaster> m_modbus;
    rclcpp::TimerBase::SharedPtr m_encoders_timer;
    rclcpp::Publisher<telerobot_interfaces::msg::Motor>::SharedPtr m_encoders_pub;
    rclcpp::Subscription<telerobot_interfaces::msg::Motor>::SharedPtr m_wheel_commands_sub;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<WheelDriver>());
  rclcpp::shutdown();
  return 0;
}