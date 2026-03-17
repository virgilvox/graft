package transports

import "sync"

// MockTransport is an in-memory transport for testing.
type MockTransport struct {
	mu          sync.Mutex
	connected   bool
	SentPackets [][]byte
	recvCh      chan []byte
}

// NewMock creates a new MockTransport.
func NewMock() *MockTransport {
	return &MockTransport{
		recvCh: make(chan []byte, 64),
	}
}

func (m *MockTransport) Connect() error {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.connected = true
	return nil
}

func (m *MockTransport) Close() error {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.connected = false
	return nil
}

func (m *MockTransport) Send(packet []byte) error {
	m.mu.Lock()
	defer m.mu.Unlock()
	cp := make([]byte, len(packet))
	copy(cp, packet)
	m.SentPackets = append(m.SentPackets, cp)
	return nil
}

func (m *MockTransport) Recv() ([]byte, error) {
	pkt := <-m.recvCh
	return pkt, nil
}

func (m *MockTransport) Connected() bool {
	m.mu.Lock()
	defer m.mu.Unlock()
	return m.connected
}

func (m *MockTransport) NeedsCOBS() bool { return false }

// Inject simulates a packet received from the device.
func (m *MockTransport) Inject(packet []byte) {
	m.recvCh <- packet
}
