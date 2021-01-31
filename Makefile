LIB_DIR = proto
SERVER_DIR = server
CLIENT_DIR = client

all: $(LIB_DIR) $(SERVER_DIR) $(CLIENT_DIR)

$(LIB_DIR):
	$(MAKE) -C $@

$(SERVER_DIR):
	$(MAKE) -C $@

$(CLIENT_DIR):
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean

.PHONY: all $(LIB_DIR) $(SERVER_DIR) $(CLIENT_DIR)