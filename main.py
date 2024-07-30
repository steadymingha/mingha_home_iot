import socket


def connect_to_ap(ip, port, password):
    try:
        # 소켓 객체 생성
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # 서버에 연결
        client_socket.connect((ip, port))

        # 비밀번호 전송
        client_socket.sendall(password.encode('utf-8'))

        # 서버로부터 인증 결과 받기
        auth_response = client_socket.recv(1024)
        if auth_response.decode('utf-8') == "OK":
            # 인증 성공 시 메시지 전송
            message = "Hello, Access Point!"
            client_socket.sendall(message.encode('utf-8'))

            # 서버로부터 응답 받기
            response = client_socket.recv(1024)
            print("Received from server:", response.decode('utf-8'))
        else:
            print("Authentication failed")

        # 소켓 닫기
        client_socket.close()

    except Exception as e:
        print(f"An error occurred: {e}")


# 주소, 포트 번호, 비밀번호 설정
ip_address = "192.168.0.14"
port_number = 80  # 실제 사용하고 있는 포트 번호로 대체
password = "!dlaudghk88"

# 함수 호출
connect_to_ap(ip_address, port_number, password)
