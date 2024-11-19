void buildFromLevelOrder(node* &root) {
    queue<node*> q; // Queue to help build the tree level by level

    // Step 1: Create the root node
    cout << "Enter data for root" << endl;
    int data;
    cin >> data;

    // Initialize the root with the input data
    root = new node(data);

    // Push the root node into the queue
    q.push(root);

    // Step 2: Process each node in the queue to add its children
    while (!q.empty()) {
        // Get the front node from the queue
        node* temp = q.front();
        q.pop();

        // Add the left child
        cout << "Enter left node for: " << temp->data << endl;
        int leftData;
        cin >> leftData;

        // If valid data is provided, create the left child and enqueue it
        if (leftData != -1) {
            temp->left = new node(leftData);
            q.push(temp->left);
        }

        // Add the right child
        cout << "Enter right node for: " << temp->data << endl;
        int rightData;
        cin >> rightData;

        // If valid data is provided, create the right child and enqueue it
        if (rightData != -1) {
            temp->right = new node(rightData);
            q.push(temp->right);
        }
    }
}
